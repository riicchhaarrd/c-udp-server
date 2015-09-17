#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#define autofree __attribute__((cleanup(free_ptr)))

static free_ptr(void *p) {
	free(*(void**)p);
}