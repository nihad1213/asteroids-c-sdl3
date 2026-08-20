#include "engine.h"

bool window_init(Window* window) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("\033[1;31mERROR: Failed to initialize SDL: %s\033[0m\n", SDL_GetError());
        return false;
    }

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

    window->renderer = SDL_CreateRenderer(window->window, NULL);

    if (!window->renderer) {
        SDL_Log("\033[1;31mERROR: Failed to create renderer: %s\033[0m\n", SDL_GetError());
        SDL_DestroyWindow(window->window);
        SDL_Quit();
        return false;
    }

    return true;
}

bool engine_init(Engine* engine) {
    if (!window_init(&engine->window)) {
        return false;
    }

    background_init(engine->window.width, engine->window.height);
    player_init(&engine->player, engine->window.width, engine->window.height);

    engine->is_running = true;
    engine->last_ticks = SDL_GetTicks();
    return true;
}

void engine_update(Engine* engine) {
    Uint64 current_ticks = SDL_GetTicks();
    float delta_time = (current_ticks - engine->last_ticks) / 1000.0f;
    engine->last_ticks = current_ticks;

    background_update(delta_time);
    player_update(&engine->player, delta_time, engine->window.width, engine->window.height);
}

void render(Engine* engine) {
    SDL_SetRenderDrawColor(engine->window.renderer, 10, 10, 15, 255);
    SDL_RenderClear(engine->window.renderer);

    background_render(engine->window.renderer);
    player_render(&engine->player, engine->window.renderer);

    SDL_RenderPresent(engine->window.renderer);
}

void input_process(Engine* engine, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        engine->is_running = false;
    } else if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE) {
            engine->is_running = false;
        }
    }
}

void engine_handle_events(Engine* engine) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        input_process(engine, &event);
    }
}

void engine_destroy(Engine* engine) {
    if (engine->window.renderer) {
        SDL_DestroyRenderer(engine->window.renderer);
    }

    if (engine->window.window) {
        SDL_DestroyWindow(engine->window.window);
    }

    SDL_Quit();
}