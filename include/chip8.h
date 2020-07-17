#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../include/opcodes.h"

#ifndef MEM_SIZE
    #define MEM_SIZE 0x1000
#endif
#ifndef REG_COUNT
    #define REG_COUNT 0x10
#endif
#ifndef STACK_SIZE
    #define STACK_SIZE 0x10
#endif
#ifndef KEYS_COUNT
    #define KEYS_COUNT 0x10
#endif
#ifndef SCREEN_SIZE
    #define SCREEN_SIZE 0x800
#endif
#ifndef GAME_ADDRESS
    #define GAME_ADDRESS 0x200
#endif

typedef struct Chip8 {
    uint8_t memory[MEM_SIZE]; // 4096 bytes of memory
    uint8_t V[REG_COUNT]; // 16 registries
    unsigned short opcode; // current opcode
    unsigned short I; // index register
    unsigned short PC; // program counter
    unsigned short stack[STACK_SIZE]; // used to remember current location
    unsigned short SP; // stack pointer
    uint8_t sound_timer; // used for sound effects
    uint8_t delay_timer; // used for timing the events of games
    uint8_t key[KEYS_COUNT]; // current state of the key
    uint8_t gfx[SCREEN_SIZE]; // black and white screen of 32*64 pixels
    bool draw_flag;
} Chip8;

extern Chip8* Chip8_factory();
extern void Chip8_loadROM(Chip8* chip8, const char* rom_path);
extern void Chip8_emulate_cycle(Chip8* chip8);
extern void Chip8_destructor(Chip8* chip8);
