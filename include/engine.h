#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    const char* title;
    int width;
    int height;
} Window;

typedef struct {
    Window window;
    bool is_running;
} Engine;

bool window_init(Window *window);