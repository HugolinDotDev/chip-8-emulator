#include <stdbool.h>

#include "../include/chip8.h"
#include "../include/graphics.h"

void emulation_loop(SDL_Window* win, Chip8* chip8)
{
    bool state = true;
    SDL_Event event;
    while (state)
    {
        Chip8_emulate_cycle(chip8);

        if (chip8->draw_flag)
            continue; // draw graphics

        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        SDL_DestroyWindow(win);
                        state = false;
                    }
                    break;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: chip8 <rom_path>\n");
        return 1;
    }
    srand(time(NULL));
    initialize_SDL();
    Chip8* chip8 = Chip8_factory();
    Chip8_loadROM(chip8, argv[1]);
    SDL_Window* win = create_window();
    emulation_loop(win, chip8);
    Chip8_destructor(chip8);
    return 0;
}