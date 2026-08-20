#pragma once

#include <SDL3/SDL.h>

typedef struct {
    float x, y;
    float vx, vy;
    float angle;
    float size;
    float invulnerable_timer;
} Player;

void player_init(Player* player, int screen_width, int screen_height);
void player_update(Player* player, float delta_time, int screen_width, int screen_height);
void player_render(Player* player, SDL_Renderer* renderer);
void player_respawn(Player* player, int screen_width, int screen_height);
void player_get_nose_position(const Player* player, float* out_x, float* out_y);
