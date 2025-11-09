#include <assert.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include "platform.h"
#include "platform_sdl.h"

static void start_sdl_platform(struct platform *platform, 
		struct platform_desc *platform_desc)
{
	struct platform_sdl_config *custom_conf = (struct platform_sdl_config *)platform_desc->config_handle;
	struct platform_sdl_config conf = {
		.init_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS,
		.exec_pre_start_hook = NULL,
		.exec_post_start_hook = NULL
	};

	assert(platform);
	if (platform_desc && platform_desc->config_handle 
			&& platform_desc->config_handle_size == sizeof(struct platform_sdl_config)) {
		conf.init_flags = custom_conf->init_flags;
		if (custom_conf->exec_pre_start_hook) {
			conf.exec_pre_start_hook = custom_conf->exec_pre_start_hook;
		}
		if (custom_conf->exec_post_start_hook) {
			conf.exec_post_start_hook = custom_conf->exec_post_start_hook;
		}
	}
	if (conf.exec_pre_start_hook) {
		conf.exec_pre_start_hook(platform);
	}
	if (!SDL_Init(conf.init_flags)) {
		return;
	}
	if (conf.exec_post_start_hook) {
		conf.exec_post_start_hook(platform);
	}
	platform->native_handle = 0; /* NOTE: Unused for this backend. */
}

static void shutdown_sdl_platform(struct platform *platform)
{
	assert(platform);
	platform->native_handle = NULL;
}

static void init_sdl_window(struct platform *platform, 
		struct platform_window_handle *window, 
		struct platform_window_desc *window_desc)
{
	struct platform_sdl_window_config *custom_conf = (struct platform_sdl_window_config *)window_desc->config_handle;
	struct platform_sdl_window_config conf = {
		.init_flags = SDL_WINDOW_RESIZABLE
	};

	assert(platform);
	assert(window);
	if (window_desc && window_desc->config_handle 
			&& window_desc->config_handle_size == sizeof(struct platform_sdl_window_config)) {
		conf.init_flags = custom_conf->init_flags;
	}
	window->native_handle = SDL_CreateWindow(window_desc->title, window_desc->width, 
			window_desc->height, conf.init_flags);
	if (!window->native_handle) {
		return;
	}
	window->parent_platform = platform;
	SDL_ShowWindow(window->native_handle);
}

static void finish_sdl_window(struct platform_window_handle *window)
{
	if (window && window->parent_platform) {
		SDL_DestroyWindow(window->native_handle);
	}
}	

void platform_sdl_bind(struct platform *platform)
{
	if (platform) {
		platform->start = start_sdl_platform;
		platform->shutdown = shutdown_sdl_platform;
		platform->init_window = init_sdl_window;
		platform->finish_window = finish_sdl_window;
	}
}
