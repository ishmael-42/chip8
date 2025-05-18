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

	uint8_t keyboard[0x10];

	framebuffer_t *fb;
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

#endif
