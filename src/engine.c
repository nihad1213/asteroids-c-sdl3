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
    asteroid_init(engine->asteroids, MAX_ASTEROIDS);
    bullet_init(engine->bullets, MAX_BULLETS);

    if (!game_init(&engine->game, engine->window.renderer, engine->asteroids, MAX_ASTEROIDS,
                   engine->window.width, engine->window.height)) {
        return false;
    }

    engine->is_running = true;
    engine->last_ticks = SDL_GetTicks();
    return true;
}

void engine_update(Engine* engine) {
    Uint64 current_ticks = SDL_GetTicks();
    float delta_time = (current_ticks - engine->last_ticks) / 1000.0f;
    engine->last_ticks = current_ticks;

    background_update(delta_time);

    if (engine->game.state == GAME_STATE_PLAYING) {
        player_update(&engine->player, delta_time, engine->window.width, engine->window.height);
    }

    game_update(&engine->game, delta_time, &engine->player,
                engine->asteroids, MAX_ASTEROIDS,
                engine->bullets, MAX_BULLETS,
                engine->window.renderer, engine->window.width, engine->window.height);
}

void render(Engine* engine) {
    SDL_SetRenderDrawColor(engine->window.renderer, 10, 10, 15, 255);
    SDL_RenderClear(engine->window.renderer);

    background_render(engine->window.renderer);
    asteroid_render(engine->asteroids, MAX_ASTEROIDS, engine->window.renderer);
    bullet_render(engine->bullets, MAX_BULLETS, engine->window.renderer);

    if (engine->game.state == GAME_STATE_PLAYING) {
        player_render(&engine->player, engine->window.renderer);
    }

    game_render(&engine->game, engine->window.renderer, engine->window.width, engine->window.height);

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
    game_destroy(&engine->game);

    if (engine->window.renderer) {
        SDL_DestroyRenderer(engine->window.renderer);
    }

    if (engine->window.window) {
        SDL_DestroyWindow(engine->window.window);
    }

    SDL_Quit();
}