#include "engine.h"
#include <stdio.h>

int main() {  
    
    Window w = {
        .title = "Asteroids",
        .width = 800,
        .height = 600
    };

    if (!window_init(&w)) {
        return 1;
    }

    return 0;
}