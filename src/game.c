#include "game.h"

#define FIRE_COOLDOWN_TIME 0.22f
#define PLAYER_COLLISION_RADIUS 12.0f
#define HUD_FONT_PATH "assets/fonts/DejaVuSansMono-Bold.ttf"
#define HUD_FONT_SIZE 18.0f
#define TITLE_FONT_SIZE 36.0f

static bool circles_overlap(float x1, float y1, float r1, float x2, float y2, float r2) {
    float dx = x1 - x2;
    float dy = y1 - y2;
    float radius_sum = r1 + r2;
    return (dx * dx + dy * dy) <= (radius_sum * radius_sum);
}

static void game_refresh_hud_texture(Game* game, SDL_Renderer* renderer) {
    char buf[64];
    SDL_snprintf(buf, sizeof(buf), "SCORE %06d  WAVE %d  LIVES %d", game->score, game->wave, game->lives);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(game->font_hud, buf, 0, white);
    if (!surface) {
        SDL_Log("\033[1;31mERROR: Failed to render HUD text: %s\033[0m\n", SDL_GetError());
        return;
    }

    if (game->hud_texture) {
        SDL_DestroyTexture(game->hud_texture);
    }
    game->hud_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
}

static void game_create_game_over_texture(Game* game, SDL_Renderer* renderer) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(game->font_title, "GAME OVER - PRESS R TO RESTART", 0, white);
    if (!surface) {
        SDL_Log("\033[1;31mERROR: Failed to render game over text: %s\033[0m\n", SDL_GetError());
        return;
    }

    if (game->game_over_texture) {
        SDL_DestroyTexture(game->game_over_texture);
    }
    game->game_over_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
}

static void game_restart(Game* game, Player* player,
                          Asteroid asteroids[], int max_asteroids,
                          Bullet bullets[], int max_bullets,
                          SDL_Renderer* renderer, int screen_width, int screen_height) {
    game->score = 0;
    game->lives = 3;
    game->wave = 1;
    game->fire_cooldown = 0.0f;

    if (game->game_over_texture) {
        SDL_DestroyTexture(game->game_over_texture);
        game->game_over_texture = NULL;
    }

    asteroid_init(asteroids, max_asteroids);
    asteroid_spawn_wave(asteroids, max_asteroids, game->wave, screen_width, screen_height);
    bullet_init(bullets, max_bullets);
    player_respawn(player, screen_width, screen_height);

    game->state = GAME_STATE_PLAYING;
    game_refresh_hud_texture(game, renderer);
}

bool game_init(Game* game, SDL_Renderer* renderer, Asteroid asteroids[], int max_asteroids, int screen_width, int screen_height) {
    if (!TTF_Init()) {
        SDL_Log("\033[1;31mERROR: Failed to initialize SDL_ttf: %s\033[0m\n", SDL_GetError());
        return false;
    }

    game->font_hud = TTF_OpenFont(HUD_FONT_PATH, HUD_FONT_SIZE);
    game->font_title = TTF_OpenFont(HUD_FONT_PATH, TITLE_FONT_SIZE);

    if (!game->font_hud || !game->font_title) {
        SDL_Log("\033[1;31mERROR: Failed to load font '%s': %s\033[0m\n", HUD_FONT_PATH, SDL_GetError());
        return false;
    }

    game->state = GAME_STATE_PLAYING;
    game->score = 0;
    game->lives = 3;
    game->wave = 1;
    game->fire_cooldown = 0.0f;
    game->hud_texture = NULL;
    game->game_over_texture = NULL;

    game_refresh_hud_texture(game, renderer);
    asteroid_spawn_wave(asteroids, max_asteroids, game->wave, screen_width, screen_height);

    return true;
}

void game_update(Game* game, float delta_time, Player* player,
                  Asteroid asteroids[], int max_asteroids,
                  Bullet bullets[], int max_bullets,
                  SDL_Renderer* renderer, int screen_width, int screen_height) {
    const bool* keys = SDL_GetKeyboardState(NULL);

    if (game->state == GAME_STATE_GAME_OVER) {
        asteroid_update(asteroids, max_asteroids, delta_time, screen_width, screen_height);
        if (keys[SDL_SCANCODE_R]) {
            game_restart(game, player, asteroids, max_asteroids, bullets, max_bullets, renderer, screen_width, screen_height);
        }
        return;
    }

    if (game->fire_cooldown > 0.0f) {
        game->fire_cooldown -= delta_time;
    }
    if (keys[SDL_SCANCODE_SPACE] && game->fire_cooldown <= 0.0f) {
        float nose_x, nose_y;
        player_get_nose_position(player, &nose_x, &nose_y);
        bullet_spawn(bullets, max_bullets, nose_x, nose_y, player->angle);
        game->fire_cooldown = FIRE_COOLDOWN_TIME;
    }

    bullet_update(bullets, max_bullets, delta_time, screen_width, screen_height);
    asteroid_update(asteroids, max_asteroids, delta_time, screen_width, screen_height);

    for (int bi = 0; bi < max_bullets; bi++) {
        if (!bullets[bi].active) continue;

        for (int ai = 0; ai < max_asteroids; ai++) {
            if (!asteroids[ai].active) continue;

            if (circles_overlap(bullets[bi].x, bullets[bi].y, BULLET_RADIUS,
                                 asteroids[ai].x, asteroids[ai].y, asteroids[ai].radius)) {
                bullets[bi].active = false;
                game->score += asteroid_hit(asteroids, max_asteroids, ai);
                game_refresh_hud_texture(game, renderer);
                break;
            }
        }
    }

    if (player->invulnerable_timer <= 0.0f) {
        for (int ai = 0; ai < max_asteroids; ai++) {
            if (!asteroids[ai].active) continue;

            if (circles_overlap(player->x, player->y, PLAYER_COLLISION_RADIUS,
                                 asteroids[ai].x, asteroids[ai].y, asteroids[ai].radius)) {
                game->lives -= 1;

                if (game->lives <= 0) {
                    game->state = GAME_STATE_GAME_OVER;
                    game_create_game_over_texture(game, renderer);
                } else {
                    player_respawn(player, screen_width, screen_height);
                }

                game_refresh_hud_texture(game, renderer);
                break;
            }
        }
    }

    if (game->state == GAME_STATE_PLAYING && asteroid_active_count(asteroids, max_asteroids) == 0) {
        game->wave += 1;
        game->score += 100 * (game->wave - 1);
        game_refresh_hud_texture(game, renderer);
        asteroid_spawn_wave(asteroids, max_asteroids, game->wave, screen_width, screen_height);
    }
}

void game_render(Game* game, SDL_Renderer* renderer, int screen_width, int screen_height) {
    if (game->hud_texture) {
        float w, h;
        SDL_GetTextureSize(game->hud_texture, &w, &h);
        SDL_FRect dst = {12.0f, 10.0f, w, h};
        SDL_RenderTexture(renderer, game->hud_texture, NULL, &dst);
    }

    if (game->state == GAME_STATE_GAME_OVER && game->game_over_texture) {
        float w, h;
        SDL_GetTextureSize(game->game_over_texture, &w, &h);
        SDL_FRect dst = {((float)screen_width - w) / 2.0f, ((float)screen_height - h) / 2.0f, w, h};
        SDL_RenderTexture(renderer, game->game_over_texture, NULL, &dst);
    }
}

void game_destroy(Game* game) {
    if (game->hud_texture) {
        SDL_DestroyTexture(game->hud_texture);
    }
    if (game->game_over_texture) {
        SDL_DestroyTexture(game->game_over_texture);
    }
    if (game->font_hud) {
        TTF_CloseFont(game->font_hud);
    }
    if (game->font_title) {
        TTF_CloseFont(game->font_title);
    }
    TTF_Quit();
}
