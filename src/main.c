#include "chip8.h"
#include "chip8_time.h"

#include "every.h"
#include "framebuffer.h"
#include "gfx.h"
#include "window_ctx.h"
#include <stdint.h>

struct chip8_render_args {
	struct renderer_t *renderer;
	framebuffer_t *framebuffer;
	struct window_ctx_t *window_ctx;
};

int chip8_render_wrapper(void *_args);
int chip8_step_wrapper(void *args);
int chip8_decrement_dt_wrapper(void *args);
int chip8_decrement_st_wrapper(void *args);

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

	float step_tick_acc = 0, dt_tick_acc = 0, st_tick_acc = 0,
	      render_tick_acc = 0;

	double last_time = chip8emu_time_now();

	struct chip8_render_args render_args = {
	    .renderer = renderer,
	    .window_ctx = window_ctx,
	    .framebuffer = &fb,
	};

	while (!window_ctx_should_stop_loop(window_ctx)) {
		double current_time = chip8emu_time_now();
		float elapsed = (float)(current_time - last_time);
		last_time = current_time;

		window_ctx_poll_events(window_ctx);
		window_ctx_set_key_state(window_ctx, &state);

		// clang-format off
		call_every_ms(1 / 120.0f  ,        elapsed,        &step_tick_acc,     chip8_step_wrapper,         (void *)&state);
		call_every_ms(1 / 60.0f   ,        elapsed,        &dt_tick_acc,       chip8_decrement_dt_wrapper, (void *)&state);
		call_every_ms(1 / 60.0f   ,        elapsed,        &st_tick_acc,       chip8_decrement_st_wrapper, (void *)&state);
		call_every_ms(1 / 60.0f   ,        elapsed,        &render_tick_acc,   chip8_render_wrapper,       (void *)&render_args);
		// clang-format on
	}

	window_ctx_deinit(window_ctx);

	return 0;
}

int chip8_render_wrapper(void *_args) {
	struct chip8_render_args *args = _args;

	renderer_draw_framebuffer(args->renderer, args->framebuffer);
	window_ctx_swap_buffers(args->window_ctx);
	return 0;
}

int chip8_step_wrapper(void *args) {
	chip8_step((chip8_state_t *)args);
	return 0;
}

int chip8_decrement_dt_wrapper(void *args) {
	chip8_decrement_dt((chip8_state_t *)args);
	return 0;
}

int chip8_decrement_st_wrapper(void *args) {
	chip8_decrement_st((chip8_state_t *)args);
	return 0;
}
