#include "window_ctx.h"

#include "GLFW/glfw3.h"
#include "chip8.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
struct window_ctx_t {
	GLFWwindow *window;
};

struct window_ctx_t *window_ctx_init(int window_width, int window_height,
                                     const char *window_title) {
	glfwInit();

	// Set the floating window hint
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	GLFWwindow *window =
	    glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);

	struct window_ctx_t *window_ctx = malloc(sizeof(struct window_ctx_t));
	window_ctx->window = window;
	return window_ctx;
}

void window_ctx_denit(struct window_ctx_t *window_ctx) {
	glfwDestroyWindow(window_ctx->window);
	glfwTerminate();
}

inline static int is_key_pressed(GLFWwindow *window, int key) {
	return (glfwGetKey(window, key) == GLFW_PRESS);
}

int window_ctx_set_key_state(struct window_ctx_t *window_ctx,
                             chip8_state_t *state) {

	glfwPollEvents();

	memset(state->keyboard, 0x00, 0x10);

	// clang-format off
	if (is_key_pressed(window_ctx->window, GLFW_KEY_1)) { state->keyboard[0x1] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_2)) { state->keyboard[0x2] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_3)) { state->keyboard[0x3] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_4)) { state->keyboard[0xC] = 1; }


	if (is_key_pressed(window_ctx->window, GLFW_KEY_Q)) { state->keyboard[0x4] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_W)) { state->keyboard[0x5] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_E)) { state->keyboard[0x6] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_R)) { state->keyboard[0xD] = 1; }

	if (is_key_pressed(window_ctx->window, GLFW_KEY_A)) { state->keyboard[0x7] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_S)) { state->keyboard[0x8] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_D)) { state->keyboard[0x9] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_F)) { state->keyboard[0xE] = 1; }

	if (is_key_pressed(window_ctx->window, GLFW_KEY_Z)) { state->keyboard[0xA] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_X)) { state->keyboard[0x0] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_C)) { state->keyboard[0xB] = 1; }
	if (is_key_pressed(window_ctx->window, GLFW_KEY_V)) { state->keyboard[0xF] = 1; }
	// clang-format on

	return 0;
}

int window_ctx_should_stop_loop(struct window_ctx_t *window_ctx) {
	return glfwWindowShouldClose(window_ctx->window);
}
