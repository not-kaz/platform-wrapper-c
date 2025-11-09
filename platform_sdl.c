#include <assert.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include "platform.h"
#include "platform_sdl.h"

static SDL_WindowFlags translate_window_feature_flags(uint32_t abstract_flags)
{
	struct feature_flag_mapping {
		SDL_WindowFlags native;
		uint32_t abstract;
	};
	const struct feature_flag_mapping flag_map[] = {
		{SDL_WINDOW_FULLSCREEN, PLATFORM_WINDOW_FEATURE_FULLSCREEN},
		{SDL_WINDOW_BORDERLESS, PLATFORM_WINDOW_FEATURE_BORDERLESS},
		{SDL_WINDOW_MINIMIZED, PLATFORM_WINDOW_FEATURE_MINIMIZED},
		{SDL_WINDOW_MAXIMIZED, PLATFORM_WINDOW_FEATURE_MAXIMIZED}
	};
	SDL_WindowFlags native_flags = 0;

	for (size_t i = 0; i < sizeof(flag_map) / sizeof(flag_map[0]); i++) {
		if (abstract_flags & flag_map[i].abstract) {
			native_flags |= flag_map[i].native;
		}
	}
	return native_flags;
}

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
	if (platform) {
		SDL_Quit();
		platform->native_handle = 0;
	}
}

static void init_sdl_window(struct platform *platform, 
		struct platform_window_handle *window, 
		struct platform_window_desc *window_desc)
{
	struct platform_sdl_window_config *custom_conf = (struct platform_sdl_window_config *)window_desc->config_handle;
	struct platform_sdl_window_config conf = {
		.init_flags = SDL_WINDOW_RESIZABLE
	};
	SDL_Window *window_handle = NULL;

	if (!platform || !window) {
		return;
	}
	if (window_desc && window_desc->config_handle 
			&& window_desc->config_handle_size == sizeof(struct platform_sdl_window_config)) {
		conf.init_flags = custom_conf->init_flags;
	}
	window_handle = SDL_CreateWindow(window_desc->title, window_desc->width, 
			window_desc->height, conf.init_flags);
	if (!window_handle) {
		return;
	}
	window->native_handle = (uintptr_t)window_handle;
	window->parent_platform = platform;
	SDL_ShowWindow((SDL_Window *)window->native_handle);
}

static void finish_sdl_window(struct platform_window_handle *window)
{
	if (window && window->parent_platform) {
		SDL_DestroyWindow((SDL_Window *)window->native_handle);
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
