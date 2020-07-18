#include "../include/chip8.h"

const unsigned char FONTSET[80] = { 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8* Chip8_factory()
{
    Chip8* chip8 = malloc(sizeof(Chip8));
    if (chip8 == NULL)
    {
        fprintf(stderr, "chip8 is a NULL pointer\n");
        exit(1);
    }
    chip8->opcode = 0;
    chip8->I = 0;
    chip8->SP = 0;
    chip8->PC = 0x200;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->draw_flag = false;
    memset(chip8->memory, 0, sizeof(uint8_t) * MEM_SIZE);
    memset(chip8->V, 0, sizeof(uint8_t) * REG_COUNT);
    memset(chip8->stack, 0, sizeof(uint8_t) * STACK_SIZE);
    memset(chip8->key, 0, sizeof(uint8_t) * KEYS_COUNT);
    memset(chip8->gfx, 0, sizeof(uint8_t) * SCREEN_SIZE);
    for (int i = 0; i < 80; i++)
        chip8->memory[i] = FONTSET[i];
    return chip8;
}

void Chip8_loadROM(Chip8* chip8, const char* rom_path)
{
    FILE* rom = fopen(rom_path, "rb");
    if (rom == NULL)
    {
        fprintf(stderr, "ROM '%s' not found\n", rom_path);
        exit(1);
    }
    fread(chip8->memory + GAME_ADDRESS, sizeof(uint8_t), MEM_SIZE, rom);
    fclose(rom);
}

void Chip8_emulate_cycle(Chip8* chip8)
{
    chip8->opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8->PC + 1]; // concat two bytes
    switch (chip8->opcode & 0xF000)
    {
        case 0x0000:
            switch (chip8->opcode & 0x00FF)
            {
                case 0x00E0: exe_cls(chip8); break; // Clears the screen
                    
                case 0x00EE: exe_ret(chip8); break; // Returns from a subroutine
            }
            break;
        case 0x1000: exe_jp_addr(chip8); break; // Jumps to address NNN
        case 0x2000: exe_call_addr(chip8); break; // Calls a subroutine at NNN
        case 0x3000: exe_se_vx_byte(chip8); break; // Skips the next instruction if VX == NN
        case 0x4000: exe_sne_vx_byte(chip8); break; // Skips the next instruction if VX != NN
        case 0x5000: exe_se_vx_vy(chip8); break; // Skips the next instruction if VX == VY
        case 0x6000: exe_ld_vx_byte(chip8); break;// Sets VX to NN
        case 0x7000: exe_add_vx_byte(chip8); break; // Adds NN to VX
        case 0x8000:
            switch (chip8->opcode & 0x000F)
            {
                case 0x0000: exe_ld_vx_vy(chip8); break; // Sets VX to the value of VY
                case 0x0001: exe_or_vx_vy(chip8); break; // Sets VX to VX OR VY
                case 0x0002: exe_and_vx_vy(chip8); break; // Sets VX to VX AND VY
                case 0x0003: exe_xor_vx_vy(chip8); break; // Sets VX to VX XOR VY
                case 0x0004: // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
                    break;
                case 0x0005: // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    break;
                case 0x0006: // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    break;
                case 0x0007: // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    break;
                case 0x000E: // Stores the most significant bit of VX in VF and then shifts VX to the left by 1
                    break;                                                                                                    
            }
            break;
        case 0x9000: // Skips the next instruction if VX != VY
            break;
        case 0xA000: // Sets I to the address NNN
            break;
        case 0xB000: // Jumps to the address NNN plus V0
            break;
        case 0xC000: // Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
            break;
        case 0xD000:// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
            break;
        case 0xE000:
            switch (chip8->opcode & 0x00FF)
                {
                case 0x009E: // Skips the next instruction if the key stored in VX is pressed
                    break;
                case 0x00A1: // Skips the next instruction if the key stored in VX isn't pressed
                    break;
            }
            break;
        case 0xF000:
            switch (chip8->opcode & 0x00FF)
            {
                case 0x0007: // Sets VX to the value of the delay timer
                    break;
                case 0x000A: // A key press is awaited, and then stored in VX
                    break;
                case 0x0015: // Sets the delay timer to VX
                    break;
                case 0x0018: // Sets the sound timer to VX
                    break;
                case 0x001E: // Adds VX to I. VF is not affected
                    break;
                case 0x0029: // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
                    break;
                case 0x0033:
                    break;
                case 0x0055:
                    break;
                case 0x0065:
                    break;
            }
            break;
    }
    if (chip8->delay_timer > 0)
        --chip8->delay_timer;
    if (chip8->sound_timer > 0)
    {
        if (chip8->sound_timer == 1)
            printf("BEEP !\n");
        --chip8->sound_timer;
    }
}

void Chip8_destructor(Chip8* chip8)
{
    free(chip8);
}
