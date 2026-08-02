#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    const char* title;
    int width;
    int height;
} Screen;

typedef struct {
    Screen screen;
    bool is_running;
} Engine;