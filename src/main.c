#include "framebuffer.h"
#include "gfx.h"
#include <stdint.h>
#include <string.h>

#include "raylib.h"

int main(void) {
	const int screenWidth = 960;
	const int screenHeight = 540;

	framebuffer_t fb;
	framebuffer_init(&fb, 64, 32);

	renderer_t renderer;
	renderer_init(&renderer, screenWidth, screenHeight, &fb);
	_framebuffer_debug_chessboard(&fb);
	framebuffer_dump_ascii(&fb);

	while (!WindowShouldClose()) {
		renderer_draw_framebuffer(&renderer, &fb);
	}

	CloseWindow();

	return 0;
}
