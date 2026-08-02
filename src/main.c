#include "engine.h"
#include <stdio.h>

int main() {  
    
    Engine e = {
        .window = {
            .title = "Asteroids",
            .width = 800,
            .height = 600
        },
        .is_running = false
    };

    if (!window_init(&e.window)) {
        return 1;
    }

    e.is_running = true;

    while (e.is_running) {
        engine_handle_events(&e);
        engine_init(&e);
    }

    engine_destroy(&e);

    return 0;
}