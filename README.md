# Asteroids

A small Asteroids clone written in C23 using SDL3.

## Features

- Ship movement with thrust, rotation, and screen wrapping
- Asteroids that split into smaller pieces when shot
- Score, waves, lives, and a game-over/restart flow
- Twinkling starfield background

## Controls

| Key | Action |
| --- | --- |
| Left / Right | Rotate ship |
| Up | Thrust |
| Space | Fire |
| R | Restart (after game over) |
| Esc | Quit |

## Building and running

Requires CMake, a C23 compiler, and the SDL3, SDL3_image, and SDL3_ttf development libraries.

```sh
cmake -S . -B build
cmake --build build
cmake --build build --target run
```

Run via the `run` target (or from the project root) so the game can find its assets in `assets/`.

## Demo

https://github.com/user-attachments/assets/6874eb93-eb1c-4ac7-8d32-a7d906bf76ae


