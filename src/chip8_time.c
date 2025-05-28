#include "chip8_time.h"

#include <time.h>

void chip8emu_time_init() {
	// no op when using C stdlib
}

double chip8emu_time_now(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}
