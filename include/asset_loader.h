#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
    SDL_Texture* texture;
    const char* path;
} BackgroundImage;