#pragma once

#include <stdlib.h>
#include <time.h>

#include "../include/chip8.h"

extern void exe_cls(Chip8* chip8); // TODO
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
extern void exe_add_vx_vy(Chip8* chip8);
extern void exe_sub_vx_vy(Chip8* chip8);
extern void exe_shr_vx_vy(Chip8* chip8);
extern void exe_subn_vx_vy(Chip8* chip8);
extern void exe_shl_vx_vy(Chip8* chip8);
extern void exe_sne_vx_vy(Chip8* chip8);
extern void exe_ld_i_addr(Chip8* chip8);
extern void exe_jp_v0_addr(Chip8* chip8);
extern void exe_rnd_vx_byte(Chip8* chip8);
extern void exe_drw_vx_vy_nibble(Chip8* chip8); // TODO
extern void exe_skp_vx(Chip8* chip8);
extern void exe_sknp_vx(Chip8* chip8);
extern void exe_ld_vx_dt(Chip8* chip8);
extern void exe_ld_vx_k(Chip8* chip8); // TODO
extern void exe_ld_dt_vx(Chip8* chip8);
extern void exe_ld_st_vx(Chip8* chip8);
extern void exe_add_i_vx(Chip8* chip8);
