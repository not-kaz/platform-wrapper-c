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

static void start_sdl_platform(struct platform *platform, struct platform_desc *desc)
{
	struct platform_sdl_override *override = NULL;
	SDL_InitFlags init_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;

	assert(platform);
	if (desc && desc->override && desc->override->handle && desc->override->size == sizeof(struct platform_sdl_override)) {
		override = (struct platform_sdl_override *)desc->override->handle;
	}
	if (override) {
		init_flags = (SDL_InitFlags)override->init_flags;
		if (override->exec_pre_start_hook) {
			override->exec_pre_start_hook(platform);
		}
	}
	if (!SDL_Init(init_flags)) {
		return;
	}
	if (override && override->exec_post_start_hook) {
		override->exec_post_start_hook(platform);
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

static void init_sdl_window(struct platform_window_handle *window, struct platform_window_desc *desc)
{
	struct platform_sdl_window_override *override = NULL;
	SDL_WindowFlags feature_flags = 0;
	SDL_Window *window_handle = NULL;

	assert(window);
	assert(window->parent_platform); /* NOTE: Not necessary, but we'll keep it here for the moment. */
	if (desc && desc->override && desc->override->handle 
			&& desc->override->size == sizeof(struct platform_sdl_window_override)) {
		override = (struct platform_sdl_window_override *)desc->override->handle;
	}
	if (override) {
		feature_flags = (SDL_WindowFlags)override->feature_flags;
	} else if (desc->feature_flags) {
		feature_flags = translate_window_feature_flags(desc->feature_flags);
	}
	window_handle = SDL_CreateWindow(desc->title, desc->width, desc->height, feature_flags);
	if (!window_handle) {
		return;
	}
	window->native_handle = (uintptr_t)window_handle;
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
