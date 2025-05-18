#include <stdint.h>

#include "raylib.h"

uint8_t framebuffer[32][64] = {};

void RenderFB(uint8_t framebuffer[32][64], Texture2D tex) {

	UpdateTexture(tex, framebuffer);

	Rectangle source = (Rectangle){
		.x = 0.0f,
		.y = 0.0f,
		.width = 64.0f,
		.height = 32.0f,
	};

	Rectangle dest = (Rectangle){
		.x = 0.0f,
		.y = 0.0f,
		.width = 960.0f,
		.height = 540.0f,
	};

	Vector2 origin = {0.0, 0.0};
	DrawTexturePro(tex, source, dest, origin, 0.0, WHITE);
}

void _ChessBoardFrameBuffer() {

	int paint = 0;

	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 64; j++) {
			framebuffer[i][j] = paint ? 255 : 0;
			paint = !paint;
		}
		paint = !paint;
	}
}

int main(void) {
	const int screenWidth = 960;
	const int screenHeight = 540;

	InitWindow(screenWidth, screenHeight,
			   "raylib [core] example - basic window");

	_ChessBoardFrameBuffer();

	Image bwImage;
	bwImage.data = framebuffer;
	bwImage.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	bwImage.width = 64;
	bwImage.height = 32;
	bwImage.mipmaps = 1;

	Texture2D renderTex = LoadTextureFromImage(bwImage);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(LIGHTGRAY);

		// DrawText("Congrats! You created your first window!", 190, 200, 20,
		// 		 DARKGRAY);

		RenderFB(framebuffer, renderTex);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
