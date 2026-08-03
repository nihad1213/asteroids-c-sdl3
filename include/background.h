#pragma once

#include <SDL3/SDL.h>
#include <stdlib.h>

#define MAX_STARS 256

typedef struct {
    float x, y;
    float size;
    Uint8 brightness;

    float twinkle_speed;
    float twinkle_phase;
} Star;

void background_init(int screen_width, int screen_height);
void background_update(float delta_time);
void background_render(SDL_Renderer* renderer);
