#include "../include/opcodes.h"

void exe_cls(Chip8* chip8)
{

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

void exe
