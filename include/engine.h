#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "background.h"
#include "player.h"
#include "asteroid.h"
#include "bullet.h"
#include "game.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    const char* title;
    int width;
    int height;
} Window;

typedef struct {
    Window window;
    Player player;
    Asteroid asteroids[MAX_ASTEROIDS];
    Bullet bullets[MAX_BULLETS];
    Game game;
    bool is_running;
    Uint64 last_ticks;
} Engine;

bool window_init(Window* window);
bool engine_init(Engine* engine);
void engine_update(Engine* engine);
void render(Engine* engine);
void engine_handle_events(Engine* engine);
void engine_destroy(Engine* engine);
void input_process(Engine* engine, SDL_Event* event);
