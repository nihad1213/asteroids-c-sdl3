#include "bullet.h"

#define BULLET_SPEED 520.0f
#define BULLET_LIFETIME 1.3f
#define BULLET_OFFSCREEN_MARGIN 40.0f

void bullet_init(Bullet bullets[], int max_count) {
    for (int i = 0; i < max_count; i++) {
        bullets[i] = (Bullet){0};
        bullets[i].active = false;
    }
}

void bullet_spawn(Bullet bullets[], int max_count, float x, float y, float angle) {
    for (int i = 0; i < max_count; i++) {
        if (!bullets[i].active) {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].vx = SDL_cosf(angle) * BULLET_SPEED;
            bullets[i].vy = SDL_sinf(angle) * BULLET_SPEED;
            bullets[i].lifetime = BULLET_LIFETIME;
            bullets[i].active = true;
            return;
        }
    }
}

void bullet_update(Bullet bullets[], int max_count, float delta_time, int screen_width, int screen_height) {
    for (int i = 0; i < max_count; i++) {
        if (!bullets[i].active) continue;

        Bullet* b = &bullets[i];
        b->x += b->vx * delta_time;
        b->y += b->vy * delta_time;
        b->lifetime -= delta_time;

        if (b->lifetime <= 0.0f) {
            b->active = false;
            continue;
        }

        if (b->x < -BULLET_OFFSCREEN_MARGIN || b->x > (float)screen_width + BULLET_OFFSCREEN_MARGIN ||
            b->y < -BULLET_OFFSCREEN_MARGIN || b->y > (float)screen_height + BULLET_OFFSCREEN_MARGIN) {
            b->active = false;
        }
    }
}

void bullet_render(const Bullet bullets[], int max_count, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);

    for (int i = 0; i < max_count; i++) {
        if (!bullets[i].active) continue;

        SDL_FRect rect = { bullets[i].x - BULLET_RADIUS, bullets[i].y - BULLET_RADIUS, BULLET_RADIUS * 2.0f, BULLET_RADIUS * 2.0f };
        SDL_RenderFillRect(renderer, &rect);
    }
}
