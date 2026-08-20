#include "asteroid.h"
#include <stdlib.h>

#define ASTEROID_LARGE_RADIUS 40.0f
#define ASTEROID_MEDIUM_RADIUS 22.0f
#define ASTEROID_SMALL_RADIUS 12.0f

#define ASTEROID_LARGE_SCORE 20
#define ASTEROID_MEDIUM_SCORE 50
#define ASTEROID_SMALL_SCORE 100

#define ASTEROID_MIN_SPEED 40.0f
#define ASTEROID_MAX_SPEED 80.0f

#define CHILD_SPEED_MULT 1.4f
#define SPLIT_SPREAD_MIN 0.35f
#define SPLIT_SPREAD_RANGE 0.45f

#define CIRCLE_SEGMENTS 14
#define WAVE_MAX_ASTEROIDS 11

static float asteroid_radius_for_size(AsteroidSize size) {
    switch (size) {
        case ASTEROID_LARGE: return ASTEROID_LARGE_RADIUS;
        case ASTEROID_MEDIUM: return ASTEROID_MEDIUM_RADIUS;
        case ASTEROID_SMALL: return ASTEROID_SMALL_RADIUS;
    }
    return ASTEROID_SMALL_RADIUS;
}

static int asteroid_score_for_size(AsteroidSize size) {
    switch (size) {
        case ASTEROID_LARGE: return ASTEROID_LARGE_SCORE;
        case ASTEROID_MEDIUM: return ASTEROID_MEDIUM_SCORE;
        case ASTEROID_SMALL: return ASTEROID_SMALL_SCORE;
    }
    return 0;
}

static int wave_asteroid_count(int wave_number) {
    int count = 4 + (wave_number - 1);
    return count > WAVE_MAX_ASTEROIDS ? WAVE_MAX_ASTEROIDS : count;
}

void asteroid_init(Asteroid asteroids[], int max_count) {
    for (int i = 0; i < max_count; i++) {
        asteroids[i] = (Asteroid){0};
        asteroids[i].active = false;
    }
}

int asteroid_spawn(Asteroid asteroids[], int max_count, float x, float y, float vx, float vy, AsteroidSize size) {
    for (int i = 0; i < max_count; i++) {
        if (!asteroids[i].active) {
            asteroids[i].x = x;
            asteroids[i].y = y;
            asteroids[i].vx = vx;
            asteroids[i].vy = vy;
            asteroids[i].radius = asteroid_radius_for_size(size);
            asteroids[i].size = size;
            asteroids[i].active = true;
            return i;
        }
    }
    return -1;
}

void asteroid_spawn_wave(Asteroid asteroids[], int max_count, int wave_number, int screen_width, int screen_height) {
    int count = wave_asteroid_count(wave_number);
    float margin = ASTEROID_LARGE_RADIUS + 10.0f;

    for (int i = 0; i < count; i++) {
        float x, y;
        int edge = rand() % 4;

        switch (edge) {
            case 0: // top
                x = (float)(rand() % screen_width);
                y = -margin;
                break;
            case 1: // bottom
                x = (float)(rand() % screen_width);
                y = (float)screen_height + margin;
                break;
            case 2: // left
                x = -margin;
                y = (float)(rand() % screen_height);
                break;
            default: // right
                x = (float)screen_width + margin;
                y = (float)(rand() % screen_height);
                break;
        }

        float angle_to_center = SDL_atan2f((screen_height / 2.0f) - y, (screen_width / 2.0f) - x);
        float spread = (((float)rand() / (float)RAND_MAX) - 0.5f) * 1.2f;
        float angle = angle_to_center + spread;
        float speed = ASTEROID_MIN_SPEED + ((float)rand() / (float)RAND_MAX) * (ASTEROID_MAX_SPEED - ASTEROID_MIN_SPEED);

        asteroid_spawn(asteroids, max_count, x, y, SDL_cosf(angle) * speed, SDL_sinf(angle) * speed, ASTEROID_LARGE);
    }
}

void asteroid_update(Asteroid asteroids[], int max_count, float delta_time, int screen_width, int screen_height) {
    for (int i = 0; i < max_count; i++) {
        if (!asteroids[i].active) continue;

        Asteroid* a = &asteroids[i];
        a->x += a->vx * delta_time;
        a->y += a->vy * delta_time;

        // Radius-aware wrap: asteroids spawn off-screen (see asteroid_spawn_wave), so the
        // bare 0/width thresholds player_update uses would wrap them back offscreen instantly.
        if (a->x < -a->radius) a->x += (float)screen_width + 2.0f * a->radius;
        if (a->x > (float)screen_width + a->radius) a->x -= (float)screen_width + 2.0f * a->radius;
        if (a->y < -a->radius) a->y += (float)screen_height + 2.0f * a->radius;
        if (a->y > (float)screen_height + a->radius) a->y -= (float)screen_height + 2.0f * a->radius;
    }
}

void asteroid_render(const Asteroid asteroids[], int max_count, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

    for (int i = 0; i < max_count; i++) {
        if (!asteroids[i].active) continue;

        const Asteroid* a = &asteroids[i];
        for (int s = 0; s < CIRCLE_SEGMENTS; s++) {
            float angle1 = (float)s * 2.0f * SDL_PI_F / (float)CIRCLE_SEGMENTS;
            float angle2 = (float)(s + 1) * 2.0f * SDL_PI_F / (float)CIRCLE_SEGMENTS;

            float x1 = a->x + SDL_cosf(angle1) * a->radius;
            float y1 = a->y + SDL_sinf(angle1) * a->radius;
            float x2 = a->x + SDL_cosf(angle2) * a->radius;
            float y2 = a->y + SDL_sinf(angle2) * a->radius;

            SDL_RenderLine(renderer, x1, y1, x2, y2);
        }
    }
}

int asteroid_hit(Asteroid asteroids[], int max_count, int index) {
    if (index < 0 || index >= max_count || !asteroids[index].active) {
        return 0;
    }

    Asteroid* a = &asteroids[index];
    int points = asteroid_score_for_size(a->size);
    float x = a->x;
    float y = a->y;
    float radius = a->radius;
    float vx = a->vx;
    float vy = a->vy;
    AsteroidSize size = a->size;
    a->active = false;

    if (size != ASTEROID_SMALL) {
        AsteroidSize child_size = (size == ASTEROID_LARGE) ? ASTEROID_MEDIUM : ASTEROID_SMALL;
        float parent_speed = SDL_sqrtf(vx * vx + vy * vy);
        float base_angle = SDL_atan2f(vy, vx);
        float child_speed = parent_speed * CHILD_SPEED_MULT;

        for (int k = 0; k < 2; k++) {
            float sign = (k == 0) ? 1.0f : -1.0f;
            float spread = sign * (SPLIT_SPREAD_MIN + ((float)rand() / (float)RAND_MAX) * SPLIT_SPREAD_RANGE);
            float child_angle = base_angle + spread;
            float cx = x + SDL_cosf(child_angle) * (radius * 0.5f);
            float cy = y + SDL_sinf(child_angle) * (radius * 0.5f);

            asteroid_spawn(asteroids, max_count, cx, cy, SDL_cosf(child_angle) * child_speed, SDL_sinf(child_angle) * child_speed, child_size);
        }
    }

    return points;
}

int asteroid_active_count(const Asteroid asteroids[], int max_count) {
    int count = 0;
    for (int i = 0; i < max_count; i++) {
        if (asteroids[i].active) count++;
    }
    return count;
}
