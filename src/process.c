#include "../include/graphics.h"

bool initialize_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        fprintf(stderr, "Failure when loading SDL (%s)\n", SDL_GetError());
        return false;
    }
    return true;
}

void emulation_loop(Chip8* chip8)
{
    Graphics* graphics = Graphics_factory("CHIP-8 Emulator");
    bool state = true;
    SDL_Event event;
    while (state)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        Graphics_destructor(graphics);
                        state = false;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_1: chip8->key[0] = 1; break;
                        case SDLK_2: chip8->key[1] = 1; break;
                        case SDLK_3: chip8->key[2] = 1; break;
                        case SDLK_4: chip8->key[3] = 1; break;
                        case SDLK_a: chip8->key[4] = 1; break;
                        case SDLK_z: chip8->key[5] = 1; break;
                        case SDLK_e: chip8->key[6] = 1; break;
                        case SDLK_r: chip8->key[7] = 1; break;
                        case SDLK_q: chip8->key[8] = 1; break;
                        case SDLK_s: chip8->key[9] = 1; break;
                        case SDLK_d: chip8->key[10] = 1; break;
                        case SDLK_f: chip8->key[11] = 1; break;
                        case SDLK_w: chip8->key[12] = 1; break;
                        case SDLK_x: chip8->key[13] = 1; break;
                        case SDLK_c: chip8->key[14] = 1; break;
                        case SDLK_v: chip8->key[15] = 1; break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_1: chip8->key[0] = 0; break;
                        case SDLK_2: chip8->key[1] = 0; break;
                        case SDLK_3: chip8->key[2] = 0; break;
                        case SDLK_4: chip8->key[3] = 0; break;
                        case SDLK_a: chip8->key[4] = 0; break;
                        case SDLK_z: chip8->key[5] = 0; break;
                        case SDLK_e: chip8->key[6] = 0; break;
                        case SDLK_r: chip8->key[7] = 0; break;
                        case SDLK_q: chip8->key[8] = 0; break;
                        case SDLK_s: chip8->key[9] = 0; break;
                        case SDLK_d: chip8->key[10] = 0; break;
                        case SDLK_f: chip8->key[11] = 0; break;
                        case SDLK_w: chip8->key[12] = 0; break;
                        case SDLK_x: chip8->key[13] = 0; break;
                        case SDLK_c: chip8->key[14] = 0; break;
                        case SDLK_v: chip8->key[15] = 0; break;                        
                    }
                    break;
            }
        }
        Chip8_emulate_cycle(chip8);
        if (chip8->draw_flag)
        {
            Graphics_draw(graphics, chip8->gfx);
            chip8->draw_flag = false;
        }
        Chip8_tick(chip8);
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
    if (!initialize_SDL())
        return 1;
    Chip8* chip8 = Chip8_factory();
    Chip8_loadROM(chip8, argv[1]);
    emulation_loop(chip8);
    Chip8_destructor(chip8);
    return 0;
}