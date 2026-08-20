#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "player.h"
#include "asteroid.h"
#include "bullet.h"

typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_GAME_OVER
} GameState;

typedef struct {
    GameState state;
    int score;
    int lives;
    int wave;
    float fire_cooldown;

    TTF_Font* font_hud;
    TTF_Font* font_title;
    SDL_Texture* hud_texture;
    SDL_Texture* game_over_texture;
} Game;

bool game_init(Game* game, SDL_Renderer* renderer, Asteroid asteroids[], int max_asteroids, int screen_width, int screen_height);
void game_update(Game* game, float delta_time, Player* player,
                  Asteroid asteroids[], int max_asteroids,
                  Bullet bullets[], int max_bullets,
                  SDL_Renderer* renderer, int screen_width, int screen_height);
void game_render(Game* game, SDL_Renderer* renderer, int screen_width, int screen_height);
void game_destroy(Game* game);
