#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef MEM_SIZE
    #define MEM_SIZE 0x1000
#endif
#ifndef REG_NB
    #define REG_NB 0x10
#endif
#ifndef STACK_SIZE
    #define STACK_SIZE 0x10
#endif
#ifndef KEYS_NB
    #define KEYS_NB 0x10
#endif
#ifndef PIX_NB
    #define PIX_NB 0x800
#endif
#ifndef GAME_ADDRESS
    #define GAME_ADDRESS 0x200
#endif

typedef struct Chip8 {
    uint8_t memory[MEM_SIZE];
    uint8_t V[REG_NB];
    uint16_t opcode;
    uint16_t I;
    uint16_t PC;
    uint16_t stack[STACK_SIZE];
    uint16_t SP;
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint8_t key[KEYS_NB];
    uint8_t gfx[PIX_NB];
    bool draw_flag;
} Chip8;

extern Chip8* Chip8_factory();
extern void Chip8_loadROM(Chip8* chip8, const char* rom_path);
extern void Chip8_emulate_cycle(Chip8* chip8);
extern void Chip8_tick(Chip8* chip8);
extern void Chip8_destructor(Chip8* chip8);
