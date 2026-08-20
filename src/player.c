#include "player.h"

#define ROTATION_SPEED 3.5f
#define THRUST_ACCEL 250.0f
#define BACK_ANGLE 2.5f

void player_init(Player* player, int screen_width, int screen_height) {
    player->x = screen_width / 2.0f;
    player->y = screen_height / 2.0f;
    player->vx = 0.0f;
    player->vy = 0.0f;
    player->angle = -SDL_PI_F / 2.0f;
    player->size = 20.0f;
}

void player_update(Player* player, float delta_time, int screen_width, int screen_height) {
    const bool* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        player->angle -= ROTATION_SPEED * delta_time;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        player->angle += ROTATION_SPEED * delta_time;
    }

    if (keys[SDL_SCANCODE_UP]) {
        player->vx += SDL_cosf(player->angle) * THRUST_ACCEL * delta_time;
        player->vy += SDL_sinf(player->angle) * THRUST_ACCEL * delta_time;
    }

    player->x += player->vx * delta_time;
    player->y += player->vy * delta_time;

    if (player->x < 0) player->x += screen_width;
    if (player->x > screen_width) player->x -= screen_width;
    if (player->y < 0) player->y += screen_height;
    if (player->y > screen_height) player->y -= screen_height;
}

void player_render(Player* player, SDL_Renderer* renderer) {
    float nose_x = player->x + SDL_cosf(player->angle) * player->size;
    float nose_y = player->y + SDL_sinf(player->angle) * player->size;

    float left_x = player->x + SDL_cosf(player->angle + BACK_ANGLE) * player->size;
    float left_y = player->y + SDL_sinf(player->angle + BACK_ANGLE) * player->size;

    float right_x = player->x + SDL_cosf(player->angle - BACK_ANGLE) * player->size;
    float right_y = player->y + SDL_sinf(player->angle - BACK_ANGLE) * player->size;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, nose_x, nose_y, left_x, left_y);
    SDL_RenderLine(renderer, left_x, left_y, right_x, right_y);
    SDL_RenderLine(renderer, right_x, right_y, nose_x, nose_y);
}
