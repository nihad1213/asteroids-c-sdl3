#include "engine.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    srand((unsigned)SDL_GetTicks());

    Engine e = {
        .window = {
            .title = "Asteroids",
            .width = 1280,
            .height = 720
        },
        .is_running = false
    };

    if (!engine_init(&e)) {
        return 1;
    }

    e.is_running = true;

    while (e.is_running) {
        engine_handle_events(&e);
        engine_update(&e);
        render(&e);
    }

    engine_destroy(&e);

    return 0;
}