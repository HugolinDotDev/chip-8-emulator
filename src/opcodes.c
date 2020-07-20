#include "../include/opcodes.h"

void exe_cls(Chip8* chip8)
{
    // TODO
}

void exe_ret(Chip8* chip8)
{
    chip8->PC = chip8->stack[chip8->SP];
    --chip8->SP;
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
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] == chip8->opcode & 0x00FF)
        chip8->PC += 2;
}

void exe_sne_vx_byte(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] != chip8->opcode & 0x00FF)
        chip8->PC += 2;
}

void exe_se_vx_vy(Chip8* chip8)
{
    if (chip8->V[(chip8->opcode >> 8) & 0x000F] == chip8->V[(chip8->opcode >> 4) & 0x000F])
        chip8->PC += 2;
}

void exe_ld_vx_byte(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] = chip8->opcode & 0x00FF;
}

void exe_add_vx_byte(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] += chip8->opcode & 0x00FF;
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
        chip8->PC += 2;
}

void exe_ld_i_addr(Chip8* chip8)
{
    chip8->I = chip8->opcode & 0x0FFF;
}

void exe_jp_v0_addr(Chip8* chip8)
{
    chip8->PC = (chip8->opcode & 0x0FFF) + chip8->V[0];
}

void exe_rnd_vx_byte(Chip8* chip8)
{
    uint8_t rnd = rand() % 0x100;
    chip8->V[(chip8->opcode >> 8) & 0x000F] = rnd & (chip8->opcode & 0x00FF);
}

void exe_drw_vx_vy_nibble(Chip8* chip8)
{
    // TODO
}

void exe_skp_vx(Chip8* chip8)
{
    if (chip8->key[chip8->V[(chip8->opcode >> 8) & 0x000F]])
        chip8->PC += 2;
}

void exe_sknp_vx(Chip8* chip8)
{
    if (!chip8->key[chip8->V[(chip8->opcode >> 8) & 0x000F]])
        chip8->PC += 2;
}

void exe_ld_vx_dt(Chip8* chip8)
{
    chip8->V[(chip8->opcode >> 8) & 0x000F] = chip8->delay_timer;
}

void exe_ld_vx_k(Chip8* chip8)
{
    // TODO
}

void exe_ld_dt_vx(Chip8* chip8)
{
    chip8->delay_timer = chip8->V[(chip8->opcode >> 8) & 0x000F];
}

void exe_ld_st_vx(Chip8* chip8)
{
    chip8->sound_timer = chip8->V[(chip8->opcode >> 8) & 0x000F];
}

void exe_add_i_vx(Chip8* chip8)
{
    chip8->I += chip8->V[(chip8->opcode >> 8) & 0x000F];
}
