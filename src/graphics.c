#include "../include/graphics.h"

void initialize_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failure when loading SDL (%s)\n", SDL_GetError());
        exit(1);
    }
}

SDL_Window* create_window()
{
    SDL_Window* win = SDL_CreateWindow("Chip8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    if (!win)
    {
        fprintf(stderr, "Failure when creating window (%s)\n", SDL_GetError());
        exit(1);
    }
    return win;
}