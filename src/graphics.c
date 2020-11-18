#include "../include/graphics.h"

Graphics* Graphics_factory(const char* win_title)
{
    Graphics* graphics = malloc(sizeof(Graphics));
    if (graphics == NULL)
    {
        fprintf(stderr, "FATAL : graphics is a null pointer\n");
        return NULL;
    }
    graphics->win = SDL_CreateWindow(win_title, 
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        SCREEN_WIDTH * PIX_SIZE, SCREEN_HEIGHT * PIX_SIZE,
                        SDL_WINDOW_SHOWN);
    if (graphics->win == NULL)
    {
        fprintf(stderr, "FATAL : Could not create window (%s)\n", SDL_GetError());
        return NULL;
    }
    graphics->rdr = SDL_CreateRenderer(graphics->win, -1,
                        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (graphics->rdr == NULL)
    {
        fprintf(stderr, "FATAL : Could not create renderer (%s)\n", SDL_GetError());
        return NULL;
    }
    SDL_SetRenderDrawColor(graphics->rdr, 0x00, 0x00, 0x00, 0xFF);
    return graphics;
}

void Graphics_draw(Graphics* graphics, uint8_t* pixels)
{
    SDL_SetRenderDrawColor(graphics->rdr, 0x00, 135, 0xFF, 0xFF);
    SDL_RenderClear(graphics->rdr);
    int row_cursor = 0, col_cursor = 0;
    for (int i = 0; i < PIX_NB; i++)
    {
        if (i % SCREEN_WIDTH == 0)
        {
            row_cursor += (1 * PIX_SIZE);
            col_cursor = 0;
        }
        if (pixels[i] == 1)
        {
            SDL_Rect rect = {
                col_cursor,
                row_cursor,
                (1 * PIX_SIZE),
                (1 * PIX_SIZE)
            };
            SDL_SetRenderDrawColor(graphics->rdr, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(graphics->rdr, &rect);
        }
        col_cursor += (1 * PIX_SIZE);
    }
    SDL_RenderPresent(graphics->rdr);
}

void Graphics_destructor(Graphics* graphics)
{
    SDL_DestroyRenderer(graphics->rdr);
    SDL_DestroyWindow(graphics->win);
    free(graphics);
}
