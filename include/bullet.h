#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

#define MAX_BULLETS 32
#define BULLET_RADIUS 2.0f

typedef struct {
    float x, y;
    float vx, vy;
    float lifetime;
    bool active;
} Bullet;

void bullet_init(Bullet bullets[], int max_count);
void bullet_spawn(Bullet bullets[], int max_count, float x, float y, float angle);
void bullet_update(Bullet bullets[], int max_count, float delta_time, int screen_width, int screen_height);
void bullet_render(const Bullet bullets[], int max_count, SDL_Renderer* renderer);
