#include "background.h"
#include <stdlib.h>

static Star starfield[MAX_STARS];

void background_init(int screen_width, int screen_height) {
    for (int i = 0; i < MAX_STARS; i++) {
        starfield[i].x = (float)(rand() % screen_width);
        starfield[i].y = (float)(rand() % screen_height);

        if (rand() % 100 < 80) {
            starfield[i].size = 1.0f;
            starfield[i].brightness = 100 + (rand() % 100);
        } else {
            starfield[i].size = (rand() % 2 == 0) ? 2.0f : 3.0f;
            starfield[i].brightness = 200 + (rand() % 56);
        }

        starfield[i].twinkle_speed = 1.0f + ((float)rand() / RAND_MAX) * 3.0f;
        starfield[i].twinkle_phase = ((float)rand() / RAND_MAX) * 6.28f;
    }
}

void background_update(float delta_time) {
    for (int i = 0; i < MAX_STARS; i++) {
        starfield[i].twinkle_phase += starfield[i].twinkle_speed * delta_time;
    }
}

void background_render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < MAX_STARS; i++) {
        Star* s = &starfield[i];

        float twinkle = 0.75f + 0.25f * SDL_sinf(s->twinkle_phase);
        Uint8 alpha = (Uint8)(s->brightness * twinkle);

        SDL_SetRenderDrawColor(renderer, 220, 230, 255, alpha);

        if (s->size <= 1.0f) {
            SDL_RenderPoint(renderer, s->x, s->y);
        } else {
            SDL_FRect rect = { s->x, s->y, s->size, s->size };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}