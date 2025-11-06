#ifndef PLATFORM_NULL_H
#define PLATFORM_NULL_H
#include <stdbool.h>

struct platform;

struct platform_null_config {
	bool should_print_actions;
};

void platform_null_bind(struct platform *platform);

#endif
