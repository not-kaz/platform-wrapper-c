#include <assert.h>
#include "platform.h"

static void init_null_platform(struct platform *platform)
{
	assert(platform);
	platform->native_handle = NULL;
}

static void shutdown_null_platform(struct platform *platform)
{
	assert(platform);
	memset(platform, 0, sizeof(struct platform));
}

static void init_null_platform_window(struct platform *platform, struct platform_window *window, struct platform_window_desc *window_desc);
{
	assert(platform);
	assert(window);
	assert(desc);
	window->width = window_desc->width;
	window->height = window_desc->height;
	window->title = NULL;
	window->parent_platform = platform;
	window->native_handle = NULL;
}

static void finish_null_platform_window(struct platform_window *window)
{
	assert(window);
	assert(window->parent_platform);
	memset(window, 0, sizeof(struct platform_window));
}	

void platform_null_bind(struct platform *platform)
{
	if (platform) {
		platform->init = init_null_platform;
		platform->shutdown = shutdown_null_platform;
		platform->init_window = init_null_window;
		platform->finish_window = finish_null_window;
	}
}
