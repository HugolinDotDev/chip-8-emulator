#pragma once

#include <SDL2/SDL.h>

#include "../include/chip8.h"

#ifndef SCREEN_WIDTH
    #define SCREEN_WIDTH 64
#endif
#ifndef SCREEN_HEIGHT
    #define SCREEN_HEIGHT 32
#endif
#ifndef PIX_SIZE
    #define PIX_SIZE 10
#endif

typedef struct Graphics {
    SDL_Window* win;
    SDL_Renderer* rdr;
} Graphics;

extern Graphics* Graphics_factory();
extern void Graphics_draw(Graphics* graphics, uint8_t* pixels);
extern void Graphics_destructor(Graphics* graphics);
