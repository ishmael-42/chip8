#include "chip8.h"
#include "chip8_instructions.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define htobe16(x) __bswap_16(x)

void chip8_state_init(chip8_state_t *state, framebuffer_t *fb) {
	state->memory = calloc(4096, sizeof(uint8_t));

	state->pc = 0x200;
	memset(state->V, 0x00, 0x10);
	state->I = 0x00;
	memset(state->stack, 0x00, 0x10);
	state->sp = 0x00;
	memset(state->keyboard, 0x00, 0x10);

	state->fb = fb;
};

static void populateRom(uint8_t *buf, FILE *file) {
	uint16_t *opcode_buffer = (uint16_t *)buf;

	/* Get the number of bytes */
	fseek(file, 0L, SEEK_END);
	size_t numbytes = ftell(file);

	/* reset the file position indicator to
	the beginning of the file */
	fseek(file, 0L, SEEK_SET);
	fread(buf, sizeof(char), numbytes, file);

	fclose(file);
}

void chip8_load_rom_file(chip8_state_t *state, const char *file_name) {
	FILE *fh = fopen(file_name, "rb");
	populateRom(state->memory + 0x200, fh);
}

static int NOT_IMPLEMENTED(chip8_state_t *state, chip8_opcode_t opcode) {
	fprintf(stderr, "Opcode 0x%04X not implemented\n", opcode);
	return -1;
}

int _forward_instruction(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t op_high = (opcode & 0xF000) >> 12;
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;
	uint8_t n = (opcode & 0x000F);
	uint8_t kk = (opcode & 0x00FF);
	uint16_t nnn = (opcode & 0x0FFF);

	switch (op_high) {
		case 0x0:
			if (opcode == 0x00E0)
				return CLS(state, opcode);
			if (opcode == 0x00EE)
				return RET(state, opcode);
			break;

		case 0x1: return JMP(state, opcode);
		case 0x2: return CALL(state, opcode);
		case 0x3: return SE(state, opcode);
		case 0x4: return SNE(state, opcode);
		case 0x5:
			if (n == 0x0)
				return SE_REG(state, opcode);
			break;
		case 0x6: return LOAD_IMMEDIATE(state, opcode);
		case 0x7: return ADD_IMMEDIATE(state, opcode);

		case 0x8:
			switch (n) {
				case 0x0: return LD_REGISTER(state, opcode);
				case 0x1: return OR_REGISTER(state, opcode);
				case 0x2: return AND_REGISTER(state, opcode);
				case 0x3: return XOR_REGISTER(state, opcode);
				case 0x4: return ADD_REGISTER(state, opcode);
				case 0x5: return SUB_REGISTER(state, opcode);
				case 0x6: return SHR(state, opcode);
				case 0x7: return SUBN_REGISTER(state, opcode);
				case 0xE: return SHL(state, opcode);
			}
			break;

		case 0x9:
			if (n == 0x0)
				return SNE_REG(state, opcode);
			break;

		case 0xA: return LOAD_I(state, opcode);
		case 0xB: return LONGJUMP(state, opcode);
		case 0xC: return RANDOM(state, opcode);
		case 0xD: return DRW(state, opcode); // Draw instruction (Dxyn)

		case 0xE:
			if (kk == 0x9E)
				return SKP_KEY(state, opcode);
			if (kk == 0xA1)
				return SKNP_KEY(state, opcode);
			break;

		case 0xF:
			switch (kk) {
				case 0x07: return NOT_IMPLEMENTED(state, opcode); // LD Vx, DT
				case 0x0A: return WAIT_FOR_K(state, opcode);
				case 0x15: return NOT_IMPLEMENTED(state, opcode); // LD DT, Vx
				case 0x18: return NOT_IMPLEMENTED(state, opcode); // LD ST, Vx
				case 0x1E: return ADD_I(state, opcode);
				case 0x29: return LD_FONT(state, opcode);
				case 0x33: return LD_BCD(state, opcode);
				case 0x55: return LD_REGS_TO_MEM(state, opcode);
				case 0x65: return LD_REGS_FROM_MEM(state, opcode);
			}
			break;
	}

	return NOT_IMPLEMENTED(state, opcode);
}

void chip8_step(chip8_state_t *state) {
	// 0. get opcode from memory, by inspecting the two bytes at state->PC (data
	// in memory from 0x200 is big endian)

	uint8_t *pc_address = &state->memory[state->pc];

	uint16_t opcode = *(uint16_t *)pc_address;
	opcode = htobe16(opcode);

	state->pc += 2;

	// 1. Create copy of current state, called next.

	chip8_state_t new_state = *state;

	// 2. Pass in the new state and opcode to the resolver.

	_forward_instruction(&new_state, opcode);

	// 3. Diff the two states to see if there's any violations (eg: stack
	// pointer not correct)
	// TODO: Maybe we should do this inside each instruction??

	*state = new_state;

	return;
}
