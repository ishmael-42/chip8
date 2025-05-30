#ifndef _CHIP8_H
#define _CHIP8_H

#include "framebuffer.h"
#include <stdint.h>

typedef struct chip8_state_t {
	uint8_t *memory;

	uint16_t pc;
	uint8_t V[0x10];
	uint16_t I;
	uint16_t stack[0x10];
	uint8_t sp;

	// Original               Emulator
	// 1 2 3 C                1 2 3 4
	// 4 5 6 D                Q W E R
	// 7 8 9 E                A S D F
	// A 0 B F                Z X C V
	uint8_t keyboard[0x10];

	framebuffer_t *fb;

	uint8_t dt, st;
} chip8_state_t;

typedef uint16_t chip8_opcode_t;

// clang-format off
static inline uint8_t  chip8_opcode_op(chip8_opcode_t op)   { return (op >> 12) & 0xF; }
static inline uint8_t  chip8_opcode_x(chip8_opcode_t op)    { return (op >> 8) & 0xF;  }
static inline uint8_t  chip8_opcode_y(chip8_opcode_t op)    { return (op >> 4) & 0xF;  }
static inline uint8_t  chip8_opcode_n(chip8_opcode_t op)    { return  op &  0xF;       }
static inline uint8_t  chip8_opcode_kk(chip8_opcode_t op)   { return  op &  0xFF;      }
static inline uint16_t chip8_opcode_nnn(chip8_opcode_t op)  { return  op &  0x0FFF;    }
// clang-format on

void chip8_state_init(chip8_state_t *state, framebuffer_t *fb);
void chip8_load_rom_file(chip8_state_t *state, const char *file_name);
void chip8_step(chip8_state_t *state);

void chip8_decrement_dt(chip8_state_t *state);
void chip8_decrement_st(chip8_state_t *state);
#endif
