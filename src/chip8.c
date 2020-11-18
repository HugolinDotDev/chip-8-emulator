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
    memset(chip8->V, 0, sizeof(uint8_t) * REG_NB);
    memset(chip8->stack, 0, sizeof(uint8_t) * STACK_SIZE);
    memset(chip8->key, 0, sizeof(uint8_t) * KEYS_NB);
    memset(chip8->gfx, 0, sizeof(uint8_t) * PIX_NB);
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

void exe_cls(Chip8* chip8)
{
    memset(chip8->gfx, 0, sizeof(uint8_t) * PIX_NB);
    chip8->draw_flag = true;
    chip8->PC += 2;
}

void exe_ret(Chip8* chip8)
{
    chip8->PC = chip8->stack[chip8->SP--];
    chip8->PC += 2;
}

void exe_jp_addr(Chip8* chip8)
{
    chip8->PC = chip8->opcode & 0x0FFF;
}

void exe_call_addr(Chip8* chip8)
{
    ++chip8->SP;
    chip8->stack[chip8->SP] = chip8->PC;
    chip8->PC = chip8->opcode & 0x0FFF;
}

void exe_se_vx_byte(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] == (chip8->opcode & 0x00FF))
        chip8->PC += 4;
    else
        chip8->PC += 2;
}

void exe_sne_vx_byte(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] != (chip8->opcode & 0x00FF))
        chip8->PC += 4;
    else
        chip8->PC += 2;
}

void exe_se_vx_vy(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] == chip8->V[(chip8->opcode >> 4) & 0x000F])
        chip8->PC += 4;
    else
        chip8->PC += 2;
}

void exe_ld_vx_byte(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] = chip8->opcode & 0x00FF;
    chip8->PC += 2;
}

void exe_add_vx_byte(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] += chip8->opcode & 0x00FF;
    chip8->PC += 2;
}

void exe_ld_vx_vy(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] = chip8->V[(chip8->opcode >> 4) & 0x000F];
}

void exe_or_vx_vy(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] |= chip8->V[(chip8->opcode >> 4) & 0x000F];
}

void exe_and_vx_vy(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] &= chip8->V[(chip8->opcode >> 4) & 0x000F];
}

void exe_xor_vx_vy(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] ^= chip8->V[(chip8->opcode >> 4) & 0x000F];
}

void exe_add_vx_vy(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] + chip8->V[(chip8->opcode >> 4) & 0x000F] > 0xFF)
        chip8->V[0xF] = 1;
    else
        chip8->V[0xF] = 0;
    chip8->V[(chip8->opcode >> 8) & 0x000F] += chip8->V[(chip8->opcode >> 4) & 0x000F];
}

void exe_sub_vx_vy(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] > chip8->V[(chip8->opcode >> 4) & 0x000F])
        chip8->V[0xF] = 1;
    else
        chip8->V[0xF] = 0;
    chip8->V[(chip8->opcode >> 8) & 0x000F] -= chip8->V[(chip8->opcode >> 4) & 0x000F];
}

void exe_shr_vx_vy(Chip8* chip8)
{
    chip8->V[0xF] = chip8->V[(chip8->opcode >> 8) & 0x0001];
    chip8->V[(chip8->opcode >> 8) & 0x000F] >>= 1;
}

void exe_subn_vx_vy(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 4) & 0x000F] > chip8->V[(chip8->opcode >> 8) & 0x000F])
        chip8->V[0xF] = 1;
    else
        chip8->V[0xF] = 0;
    chip8->V[(chip8->opcode >> 8) & 0x000F] =
        chip8->V[(chip8->opcode >> 4) & 0x000F] - chip8->V[(chip8->opcode >> 8) & 0x000F];
}

void exe_shl_vx_vy(Chip8* chip8)
{
    chip8->V[0xF] = (chip8->V[(chip8->opcode >> 8) & 0x000F] >> 7) & 0x1;
    chip8->V[(chip8->opcode >> 8) & 0x000F] <<= 1;
}

void exe_sne_vx_vy(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] != chip8->V[(chip8->opcode >> 4) & 0x000F])
        chip8->PC += 4;
    else
        chip8->PC += 2;
}

void exe_ld_i_addr(Chip8* chip8)
{
    chip8->I = chip8->opcode & 0x0FFF;
    chip8->PC += 2;
}

void exe_jp_v0_addr(Chip8* chip8)
{
    chip8->PC = (chip8->opcode & 0x0FFF) + chip8->V[0];
}

void exe_rnd_vx_byte(Chip8* chip8)
{
    uint8_t rnd = rand() % 0x100;
    chip8->V[(chip8->opcode >> 8) & 0x000F] = rnd & (chip8->opcode & 0x00FF);
    chip8->PC += 2;
}

void exe_drw_vx_vy_nibble(Chip8* chip8)
{
    uint8_t x_pos = chip8->V[(chip8->opcode >> 8) & 0x000F];
    uint8_t y_pos = chip8->V[(chip8->opcode >> 4) & 0x000F];
    uint8_t height = chip8->opcode & 0x000F;
    uint8_t pixel;
    chip8->V[0xF] = 0;
    for (uint8_t row = 0; row < height; row++)
    {
        pixel = chip8->memory[chip8->I + row];
        for (uint8_t col = 0; col < 8; col++)
        {
            if ((pixel & (0x80 >> col)) != 0)
            {
                if (chip8->gfx[x_pos + col + ((y_pos + row) * 64)] == 1)
                    chip8->V[0xF] = 1;
                chip8->gfx[x_pos + col + ((y_pos + row) * 64)] ^= 1;
            }
        }
    }
    chip8->draw_flag = true;
    chip8->PC += 2;
}

void exe_skp_vx(Chip8* chip8)
{
    if (chip8->key[chip8->V[(chip8->opcode >> 8) & 0x000F]])
        chip8->PC += 4;
    else
        chip8->PC += 2;
}

void exe_sknp_vx(Chip8* chip8)
{
    if (!chip8->key[chip8->V[(chip8->opcode >> 8) & 0x000F]])
        chip8->PC += 4;
    else
        chip8->PC += 2;
}

void exe_ld_vx_dt(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] = chip8->delay_timer;
    chip8->PC += 2;
}

void exe_ld_vx_k(Chip8* chip8)
{
    bool state = true;
    while (true)
    {
        for (int i = 0; i < KEYS_NB; i++)
        {
            if (chip8->key[i])
            {
                chip8->V[(chip8->opcode >> 8) & 0x000F] = i;
                state = false;
            }
        }
    }
    chip8->PC += 2;
}

void exe_ld_dt_vx(Chip8* chip8)
{
    chip8->delay_timer = chip8->V[(chip8->opcode >> 8) & 0x000F];
    chip8->PC += 2;
}

void exe_ld_st_vx(Chip8* chip8)
{
    chip8->sound_timer = chip8->V[(chip8->opcode >> 8) & 0x000F];
    chip8->PC += 2;
}

void exe_add_i_vx(Chip8* chip8)
{
    chip8->I += chip8->V[(chip8->opcode >> 8) & 0x000F];
    chip8->PC += 2;
}

void exe_ld_f_vx(Chip8* chip8)
{
    chip8->I = chip8->V[(chip8->opcode >> 8) & 0x000F] * 5;
    chip8->PC += 2;
}

void exe_ld_b_vx(Chip8* chip8)
{
    uint8_t x = (chip8->opcode >> 8) & 0x000F;
    chip8->memory[chip8->I] = (chip8->V[x] % 1000) / 100;
    chip8->memory[chip8->I + 1] = (chip8->V[x] % 100) / 10;
    chip8->memory[chip8->I + 2] = chip8->V[x] % 10;
    chip8->PC += 2;
}

void exe_ld_i_vx(Chip8* chip8)
{
    for (int i = 0; i <= ((chip8->opcode >> 8) & 0x000F); i++)
        chip8->memory[chip8->I + i] = chip8->V[i];
    chip8->I += ((chip8->opcode >> 8) & 0x000F) + 1;
    chip8->PC += 2;
}

void exe_ld_vx_i(Chip8* chip8)
{
    for (int i = 0; i <= ((chip8->opcode >> 8) & 0x000F); i++)
        chip8->V[i] = chip8->memory[chip8->I + i];
    chip8->I += ((chip8->opcode >> 8) & 0x000F) + 1;
    chip8->PC += 2;
}

void Chip8_emulate_cycle(Chip8* chip8)
{
    chip8->opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8->PC + 1]; // concat two bytes
    switch (chip8->opcode & 0xF000)
    {
        case 0x0000:
            switch (chip8->opcode & 0x00FF)
            {
                case 0x00E0: exe_cls(chip8); break;
                    
                case 0x00EE: exe_ret(chip8); break;
            }
            break;
        case 0x1000: exe_jp_addr(chip8); break;
        case 0x2000: exe_call_addr(chip8); break;
        case 0x3000: exe_se_vx_byte(chip8); break;
        case 0x4000: exe_sne_vx_byte(chip8); break;
        case 0x5000: exe_se_vx_vy(chip8); break;
        case 0x6000: exe_ld_vx_byte(chip8); break;
        case 0x7000: exe_add_vx_byte(chip8); break;
        case 0x8000:
            switch (chip8->opcode & 0x000F)
            {
                case 0x0000: exe_ld_vx_vy(chip8); break;
                case 0x0001: exe_or_vx_vy(chip8); break;
                case 0x0002: exe_and_vx_vy(chip8); break;
                case 0x0003: exe_xor_vx_vy(chip8); break;
                case 0x0004: exe_add_vx_vy(chip8); break;
                case 0x0005: exe_sub_vx_vy(chip8); break;
                case 0x0006: exe_shr_vx_vy(chip8); break;
                case 0x0007: exe_subn_vx_vy(chip8); break;
                case 0x000E: exe_shl_vx_vy(chip8); break;
            }
            chip8->PC += 2;
            break;
        case 0x9000: exe_sne_vx_vy(chip8); break;
        case 0xA000: exe_ld_i_addr(chip8); break;
        case 0xB000: exe_jp_v0_addr(chip8); break;
        case 0xC000: exe_rnd_vx_byte(chip8); break;
        case 0xD000: exe_drw_vx_vy_nibble(chip8); break;
        case 0xE000:
            switch (chip8->opcode & 0x00FF)
                {
                case 0x009E: exe_skp_vx(chip8); break;
                case 0x00A1: exe_sknp_vx(chip8); break;
            }
            break;
        case 0xF000:
            switch (chip8->opcode & 0x00FF)
            {
                case 0x0007: exe_ld_vx_dt(chip8); break;
                case 0x000A: exe_ld_vx_k(chip8); break;
                case 0x0015: exe_ld_dt_vx(chip8); break;
                case 0x0018: exe_ld_st_vx(chip8); break;
                case 0x001E: exe_add_i_vx(chip8); break;
                case 0x0029: exe_ld_f_vx(chip8); break;
                case 0x0033: exe_ld_b_vx(chip8); break;
                case 0x0055: exe_ld_i_vx(chip8); break;
                case 0x0065: exe_ld_vx_i(chip8); break;
            }
            break;
    }
}

void Chip8_tick(Chip8* chip8)
{
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
