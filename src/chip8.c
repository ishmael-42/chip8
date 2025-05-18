#include "chip8.h"
#include <stdlib.h>
#include <string.h>

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
