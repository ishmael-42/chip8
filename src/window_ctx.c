#include "chip8_config.h"

#ifdef USE_RAYLIB
#include "window_ctx.raylib.c"
#else
#include "window_ctx.glfw.c"
#endif
