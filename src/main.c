#include "chip8.h"
#include "framebuffer.h"
#include "gfx.h"
#include <stdint.h>
#include <stdio.h>

#include "raylib.h"
// clang-format off
uint8_t image_0[] = {
	0b11110000,
	0b10010000,
	0b10010000,
	0b10010000,
	0b11110000,
};
// clang-format on

int main(void) {
	const int screenWidth = 960;
	const int screenHeight = 540;

	framebuffer_t fb;
	framebuffer_init(&fb, 64, 32);

	struct renderer_t *renderer;
	renderer_init(&renderer, screenWidth, screenHeight, &fb);

	chip8_state_t state;
	chip8_state_init(&state, &fb);
	chip8_load_rom_file(&state, "./output.ch8");

	while (!WindowShouldClose()) {
		chip8_step(&state);
		renderer_draw_framebuffer(renderer, &fb);
	}

	CloseWindow();

	return 0;
}
