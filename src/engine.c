#include "engine.h"

bool window_init(Window* window) {
    SDL_Init(SDL_INIT_VIDEO);

    window->window = SDL_CreateWindow(
        window->title,
        window->width, 
        window->height, 
        SDL_WINDOW_RESIZABLE
    );

    if (!window->window) {
        SDL_Log("\033[1;31mERROR: Failed to create window: %s \033[0m\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

void window_destroy(Window* window) {
    if (window->window) {
        SDL_DestroyWindow(window->window);
    }

    SDL_Quit();
}