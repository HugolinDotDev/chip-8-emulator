#pragma once

#include "../include/chip8.h"

extern void exe_cls(Chip8* chip8);
extern void exe_ret(Chip8* chip8);
extern void exe_jp_addr(Chip8* chip8);
extern void exe_call_addr(Chip8* chip8);
extern void exe_se_vx_byte(Chip8* chip8);
extern void exe_sne_vx_byte(Chip8* chip8);
extern void exe_se_vx_vy(Chip8* chip8);
extern void exe_ld_vx_byte(Chip8* chip8);
extern void exe_add_vx_byte(Chip8* chip8);
extern void exe_ld_vx_vy(Chip8* chip8);
extern void exe_or_vx_vy(Chip8* chip8);
extern void exe_and_vx_vy(Chip8* chip8);
extern void exe_xor_vx_vy(Chip8* chip8);
