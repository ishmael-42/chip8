#include "window_ctx.h"

#include <raylib.h>
#include <stdlib.h>
#include <string.h>
struct window_ctx_t {};

struct window_ctx_t *window_ctx_init(int window_width, int window_height,
                                     const char *window_title) {
	InitWindow(window_width, window_height, window_title);
	struct window_ctx_t *dummy = malloc(sizeof(struct window_ctx_t));
	return dummy;
}

void window_ctx_deinit(struct window_ctx_t *_window_ctx) {
	CloseWindow();
}

// Original               Emulator
// 1 2 3 C                1 2 3 4
// 4 5 6 D                Q W E R
// 7 8 9 E                A S D F
// A 0 B F                Z X C V
int window_ctx_set_key_state(struct window_ctx_t *_window_ctx,
                             chip8_state_t *state) {

	memset(state->keyboard, 0x00, 0x10);

	// clang-format off
	if (IsKeyPressed(KEY_ONE)) { state->keyboard[0x1] = 1; }
	if (IsKeyPressed(KEY_TWO)) { state->keyboard[0x2] = 1; }
	if (IsKeyPressed(KEY_THREE)) { state->keyboard[0x3] = 1; }
	if (IsKeyPressed(KEY_FOUR)) { state->keyboard[0xC] = 1; }


	if (IsKeyPressed(KEY_Q)) { state->keyboard[0x4] = 1; }
	if (IsKeyPressed(KEY_W)) { state->keyboard[0x5] = 1; }
	if (IsKeyPressed(KEY_E)) { state->keyboard[0x6] = 1; }
	if (IsKeyPressed(KEY_R)) { state->keyboard[0xD] = 1; }

	if (IsKeyPressed(KEY_A)) { state->keyboard[0x7] = 1; }
	if (IsKeyPressed(KEY_S)) { state->keyboard[0x8] = 1; }
	if (IsKeyPressed(KEY_D)) { state->keyboard[0x9] = 1; }
	if (IsKeyPressed(KEY_F)) { state->keyboard[0xE] = 1; }

	if (IsKeyPressed(KEY_Z)) { state->keyboard[0xA] = 1; }
	if (IsKeyPressed(KEY_X)) { state->keyboard[0x0] = 1; }
	if (IsKeyPressed(KEY_C)) { state->keyboard[0xB] = 1; }
	if (IsKeyPressed(KEY_V)) { state->keyboard[0xF] = 1; }
	// clang-format on

	return 0;
}
int window_ctx_should_stop_loop(struct window_ctx_t *_window_ctx) {
	return WindowShouldClose();
};
