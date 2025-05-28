#include "every.h"

int call_every_ms(float interval, float elapsed, float *acc,
                  callback_func_t callback_func, void *args) {
	*acc += elapsed;

	while (*acc >= interval) {
		*acc -= interval;
		return callback_func(args);
	}

	return 0;
}
