#ifndef WINDOW_CTX
#define WINDOW_CTX

#include "chip8.h"
struct window_ctx_t;

struct window_ctx_t *window_ctx_init(int window_width, int window_height,
                                     const char *window_title);
void window_ctx_denit(struct window_ctx_t *);

int window_ctx_set_key_state(struct window_ctx_t *, chip8_state_t *state);
int window_ctx_should_stop_loop(struct window_ctx_t *);

#endif // !WINDOW_CTX
