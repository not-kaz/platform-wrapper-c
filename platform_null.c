#include <assert.h>
#include "platform.h"

static void start_null_platform(struct platform *platform)
{
	assert(platform);
	platform->native_handle = NULL;
}

static void shutdown_null_platform(struct platform *platform)
{
	assert(platform);
	platform->native_handle = NULL;
}

static void init_null_window(struct platform *platform, 
		struct platform_window_handle *window, 
		struct platform_window_desc *window_desc)
{
	assert(platform);
	assert(window);
	assert(window_desc);
	window->parent_platform = platform;
	window->native_handle = NULL;
}

static void finish_null_window(struct platform_window_handle *window)
{
	assert(window);
	assert(window->parent_platform);
	window->native_handle = NULL;
}	

void platform_null_bind(struct platform *platform)
{
	if (platform) {
		platform->start = start_null_platform;
		platform->shutdown = shutdown_null_platform;
		platform->init_window = init_null_window;
		platform->finish_window = finish_null_window;
	}
}
