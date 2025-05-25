#include "chip8.h"
#include "every.h"
#include "framebuffer.h"
#include "gfx.h"
#include "window_ctx.h"
#include <stdint.h>

// clang-format off
uint8_t image_0[] = {
	0b11110000,
	0b10010000,
	0b10010000,
	0b10010000,
	0b11110000,
};
// clang-format on

int main(int argc, char *argv[argc + 1]) {
	const int screenWidth = 960;
	const int screenHeight = 960 / 2;

	struct window_ctx_t *window_ctx =
	    window_ctx_init(screenWidth, screenHeight, "Hahaha");

	framebuffer_t fb;
	framebuffer_init(&fb, 64, 32);

	struct renderer_t *renderer;
	renderer_init(&renderer, screenWidth, screenHeight, &fb);

	chip8_state_t state;
	chip8_state_init(&state, &fb);
	chip8_load_rom_file(&state, argv[1]);

	while (!window_ctx_should_stop_loop(window_ctx)) {
		window_ctx_set_key_state(window_ctx, &state);
		chip8_step(&state);
		renderer_draw_framebuffer(renderer, &fb);
	}

	window_ctx_denit(window_ctx);

	return 0;
}
