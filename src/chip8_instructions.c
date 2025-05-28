#include "chip8_instructions.h"
#include "chip8.h"
#include "framebuffer.h"
#include <stddef.h>
#include <stdint.h>

/*
 * An instruction is a function, that takes in the current chip8 state, and
 * returns the new chip8 state, and error codes if any.
 *
 * Default signature:
 *
 * int instruction(chip8_state_t *state, chip8_opcode opcode);
 *
 */

// 6xkk - LD Vx, byte
int LOAD_IMMEDIATE(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t kk = chip8_opcode_kk(opcode);

	state->V[x] = kk;
	return 0;
}

// 7xkk - ADD Vx, byte
int ADD_IMMEDIATE(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t kk = chip8_opcode_kk(opcode);

	state->V[x] += kk;
	return 0;
}

// 8xy0 - LD Vx, Vy
int LD_REGISTER(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	state->V[x] = state->V[y];
	return 0;
}

// 8xy1 - OR Vx, Vy
int OR_REGISTER(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	state->V[x] |= state->V[y];
	return 0;
}

// 8xy2 - AND Vx, Vy
int AND_REGISTER(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	state->V[x] &= state->V[y];
	return 0;
}

// 8xy3 - XOR Vx, Vy
int XOR_REGISTER(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	state->V[x] ^= state->V[y];
	return 0;
}

// 8xy4 - ADD Vx, Vy with carry
int ADD_REGISTER(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	uint16_t result = state->V[x] + state->V[y];
	state->V[0xF] = result > 0xFF; // set carry flag
	state->V[x] = result & 0xFF;
	return 0;
}

// 8xy5 - SUB Vx, Vy
int SUB_REGISTER(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	state->V[0xF] = state->V[x] > state->V[y]; // set borrow flag
	state->V[x] = state->V[x] - state->V[y];
	return 0;
}

// 8xy7 - SUBN Vx, Vy
int SUBN_REGISTER(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	state->V[0xF] = state->V[y] > state->V[x]; // set borrow flag
	state->V[x] = state->V[y] - state->V[x];
	return 0;
}

// Fx1E - ADD I, Vx
int ADD_I(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	state->I += state->V[x];
	return 0;
}

// Fx29 - LD F, Vx
int LD_FONT(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t digit = state->V[x];

	// Each font sprite is 5 bytes long, typically starts at memory[0x0]
	state->I = digit * 5;
	return 0;
}

// Fx33 - LD B, Vx
int LD_BCD(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t value = state->V[x];

	state->memory[state->I] = value / 100;
	state->memory[state->I + 1] = (value / 10) % 10;
	state->memory[state->I + 2] = value % 10;
	return 0;
}

// Fx55 - LD [I], Vx
int LD_REGS_TO_MEM(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);

	for (uint8_t i = 0; i <= x; i++) {
		state->memory[state->I + i] = state->V[i];
	}
	return 0;
}

// Fx65 - LD Vx, [I]
int LD_REGS_FROM_MEM(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);

	for (uint8_t i = 0; i <= x; i++) {
		state->V[i] = state->memory[state->I + i];
	}
	return 0;
}

// 00E0 - CLS
int CLS(chip8_state_t *state, chip8_opcode_t _opcode) {
	framebuffer_fill_color(state->fb, 0x00);
	return 0;
}

// 00EE - RET
int RET(chip8_state_t *state, chip8_opcode_t _opcode) {
	uint8_t new_pc = state->stack[state->sp];
	state->pc = new_pc;
	state->sp -= 1;
	return 0;
}

// 1nnn - JP addr
int JMP(chip8_state_t *state, chip8_opcode_t opcode) {
	uint16_t nnn = chip8_opcode_nnn(opcode);
	state->pc = nnn;
	return 0;
}

// 2nnn - CALL addr
int CALL(chip8_state_t *state, chip8_opcode_t opcode) {
	uint16_t nnn = chip8_opcode_nnn(opcode);
	state->sp += 1;
	state->stack[state->sp] = state->pc;
	state->pc = nnn;
	return 0;
}

int SYS(chip8_state_t *state, chip8_opcode_t opcode) {
	// TODO: The refernce says it's ok to skip this, but is it?
	return 0;
}

// 3xkk - SE Vx, byte
int SE(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t kk = chip8_opcode_kk(opcode);

	if (state->V[x] == kk) {
		state->pc += 2;
	}
	return 0;
}

// 4xkk - SNE Vx, byte
int SNE(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t kk = chip8_opcode_kk(opcode);

	if (state->V[x] != kk) {
		state->pc += 2;
	}
	return 0;
}

// 5xy0 - SE Vx, Vy
int SE_REG(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	if (state->V[x] == state->V[y]) {
		state->pc += 2;
	}
	return 0;
}

// 8xy6 - SHR Vx {, Vy}
// If the least-significant bit of prior to the shift Vx is 1, then VF is set to
// 1, otherwise 0.
int SHR(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);

	state->V[0xF] = state->V[x] & 0x01;
	state->V[x] >>= 1;

	return 0;
}

// 8xyE - SHL Vx {, Vy}
//  If the most-significant bit of Vx prior to the shift is 1, then VF is set to
//  1, otherwise to 0.
int SHL(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);

	state->V[0xF] = state->V[x] >> 7;
	state->V[x] <<= 1;

	return 0;
}

// 9xy0 - SE Vx, Vy
int SNE_REG(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);

	if (state->V[x] != state->V[y]) {
		state->pc += 2;
	}
	return 0;
}

// Annn - LD I, addr
int LOAD_I(chip8_state_t *state, chip8_opcode_t opcode) {
	uint16_t nnn = chip8_opcode_nnn(opcode);
	state->I = nnn;
	return 0;
}

// Bnnn - JP V0, addr
int LONGJUMP(chip8_state_t *state, chip8_opcode_t opcode) {
	uint16_t nnn = chip8_opcode_nnn(opcode);
	state->pc = state->V[0] + nnn;
	return 0;
}

// Cxkk - RND Vx, byte
// TODO: inject RNG here.
int RANDOM(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	uint8_t kk = chip8_opcode_kk(opcode);

	// for now, let's use PC as the random number

	state->V[x] = state->pc & kk;
	return 0;
}

// Ex9E - SKP Vx
int SKP_KEY(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);

	if (state->keyboard[x]) {
		state->pc += 2;
	}

	return 0;
}

// ExA1 - SKP Vx
int SKNP_KEY(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);

	if (!state->keyboard[x]) {
		state->pc += 2;
	}

	return 0;
}

// Fx0A - LD Vx, K
int WAIT_FOR_K(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);

	int8_t pressed_key = -1;
	for (int i = 0; i < 0x10; i++) {
		if (state->keyboard[i]) {
			pressed_key = i;
		}
	}

	if (pressed_key == -1) {
		state->pc -= 2; // simulate waiting by executing the same instruction
	} else {
		state->V[x] = pressed_key;
	}

	return 0;
}

// Dxyn - DRW Vx, Vy, nibble

int DRW(chip8_state_t *state, chip8_opcode_t opcode) {

	uint8_t x = chip8_opcode_x(opcode);
	uint8_t y = chip8_opcode_y(opcode);
	uint8_t n = chip8_opcode_n(opcode);

	uint8_t *sprite_start_location = state->memory + (size_t)state->I;

	uint8_t x_pos = state->V[x];
	uint8_t y_pos = state->V[y];

	// uint8_t did_overwrite =
	// framebuffer_draw_image(state->fb, x, y, n, sprite_start_location);
	uint8_t did_overwrite = framebuffer_draw_image(state->fb, x_pos, y_pos, n,
	                                               sprite_start_location);

	state->V[0xF] = did_overwrite;
	return 0;
}

// Fx07 - LD Vx, DT
int LD_VX_DT(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	state->V[x] = state->dt;
	return 0;
}

// Fx15 - LD DT, Vx
int LD_DT_VX(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	state->dt = state->V[x];
	return 0;
}

// Fx18 - LD ST, Vx
int LD_ST_VX(chip8_state_t *state, chip8_opcode_t opcode) {
	uint8_t x = chip8_opcode_x(opcode);
	state->st = state->V[x];
	return 0;
}
