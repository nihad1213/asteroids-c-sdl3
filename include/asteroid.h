#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

#define MAX_ASTEROIDS 64

typedef enum {
    ASTEROID_LARGE,
    ASTEROID_MEDIUM,
    ASTEROID_SMALL
} AsteroidSize;

typedef struct {
    float x, y;
    float vx, vy;
    float radius;
    AsteroidSize size;
    bool active;
} Asteroid;

void asteroid_init(Asteroid asteroids[], int max_count);
void asteroid_spawn_wave(Asteroid asteroids[], int max_count, int wave_number, int screen_width, int screen_height);
int asteroid_spawn(Asteroid asteroids[], int max_count, float x, float y, float vx, float vy, AsteroidSize size);
void asteroid_update(Asteroid asteroids[], int max_count, float delta_time, int screen_width, int screen_height);
void asteroid_render(const Asteroid asteroids[], int max_count, SDL_Renderer* renderer);
int asteroid_hit(Asteroid asteroids[], int max_count, int index);
int asteroid_active_count(const Asteroid asteroids[], int max_count);
