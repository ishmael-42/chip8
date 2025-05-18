#include "chip8.h"
#include "chip8_instructions.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
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

