#ifndef INCLUDE_EVERY_H
#define INCLUDE_EVERY_H

typedef int (*callback_func_t)(void *);

int call_every_ms(float interval, float elapsed, float *acc,
                  callback_func_t callback_func, void *args);

#endif
