#include <SDL3/SDL.h>
#include <stdio.h>
#include "platform.h"
#include "platform_sdl.h"

static SDL_WindowFlags translate_window_feature_flags(uint32_t abstract_flags)
{
	const struct {
		SDL_WindowFlags native;
		uint32_t abstract;
	} flag_map[] = {
		{SDL_WINDOW_RESIZABLE, PLATFORM_WINDOW_FEATURE_RESIZABLE},
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

static uintptr_t create_sdl_backend(void)
{
	SDL_InitFlags init_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;

	if (!SDL_Init(init_flags)) {
		return (uintptr_t)NULL;
	}
	return (uintptr_t)1; /* Token value since SDL has no explicit state. */
}

static void destroy_sdl_backend(const uintptr_t platform)
{
	(void)platform; /* Unused on SDL. */
	SDL_Quit();
}

static bool poll_sdl_event(struct platform_event *event_out, 
		const uintptr_t platform)
{
	SDL_Event native_event;

	(void)platform;
	if (!SDL_PollEvent(&native_event)) {
		return false;
	}
	switch (native_event.type) {
	/* Application events */
	case SDL_EVENT_QUIT:
		event_out->type = PLATFORM_EVENT_TYPE_QUIT;
		break;
	/* Window events */
	case SDL_EVENT_WINDOW_SHOWN:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_SHOWN;
		break;
	case SDL_EVENT_WINDOW_HIDDEN:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_HIDDEN;
		break;
	case SDL_EVENT_WINDOW_RESIZED:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_RESIZED;
		break;
	case SDL_EVENT_WINDOW_MINIMIZED:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_MINIMIZED;
		break;
	case SDL_EVENT_WINDOW_MAXIMIZED:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_MAXIMIZED;
		break;
	case SDL_EVENT_WINDOW_DESTROYED:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_DESTROYED;
		break;
	case SDL_EVENT_WINDOW_FOCUS_GAINED:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_FOCUS_GAINED;
		break;
	case SDL_EVENT_WINDOW_FOCUS_LOST:
		event_out->type = PLATFORM_EVENT_TYPE_WINDOW_FOCUS_LOST;
		break;
	/* Keyboard events */
	case SDL_EVENT_KEY_DOWN:
		event_out->type = PLATFORM_EVENT_TYPE_KEY_DOWN;
		break;
	case SDL_EVENT_KEY_UP:
		event_out->type = PLATFORM_EVENT_TYPE_KEY_UP;
		break;
	case SDL_EVENT_KEYBOARD_ADDED:
		event_out->type = PLATFORM_EVENT_TYPE_KEYBOARD_ADDED;
		break;
	case SDL_EVENT_KEYBOARD_REMOVED:
		event_out->type = PLATFORM_EVENT_TYPE_KEYBOARD_REMOVED;
		break;
	/* Mouse events */
	case SDL_EVENT_MOUSE_MOTION:
		event_out->type = PLATFORM_EVENT_TYPE_MOUSE_MOTION;
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		event_out->type = PLATFORM_EVENT_TYPE_MOUSE_BUTTON_DOWN;
		break;
	case SDL_EVENT_MOUSE_BUTTON_UP:
		event_out->type = PLATFORM_EVENT_TYPE_MOUSE_BUTTON_UP;
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		event_out->type = PLATFORM_EVENT_TYPE_MOUSE_WHEEL;
		break;
	case SDL_EVENT_MOUSE_ADDED:
		event_out->type = PLATFORM_EVENT_TYPE_MOUSE_ADDED;
		break;
	case SDL_EVENT_MOUSE_REMOVED:
		event_out->type = PLATFORM_EVENT_TYPE_MOUSE_REMOVED;
		break;
	default:
		event_out->type = PLATFORM_EVENT_TYPE_UNDEFINED;
		break;
	}
	return true;
}

static uintptr_t create_sdl_window(const struct platform_window_desc *desc,
		const uintptr_t platform)
{
	SDL_WindowFlags feature_flags;
	SDL_Window *native_window;

	(void)platform;
	feature_flags = translate_window_feature_flags(desc->feature_flags);
	native_window = SDL_CreateWindow(desc->title, desc->width, desc->height, 
			feature_flags);
	if (!native_window) {
		return (uintptr_t)NULL;
	}
	return (uintptr_t)native_window;
}

static void destroy_sdl_window(uintptr_t window)
{
	SDL_DestroyWindow((SDL_Window *)window);
}

static uintptr_t create_sdl_surface(const struct platform_surface_desc *desc, 
		const uintptr_t platform)
{
	SDL_Surface *native_surface;

	(void)platform;
	/* TODO: Settle on proper contract for the pitch, possibly something portable. *
	 * Right now it's 'width * 4', which aligns with the 'BGRA8888' format. */
	native_surface = SDL_CreateSurfaceFrom(desc->width, desc->height, 
			SDL_PIXELFORMAT_BGRA8888, desc->pixel_buffer,
			desc->width * 4); 
	if (!native_surface) {
		return (uintptr_t)NULL;
	}
	return (uintptr_t)native_surface;
}

static void destroy_sdl_surface(const uintptr_t surface)
{
	SDL_DestroySurface((SDL_Surface *)surface);
}

static void blit_sdl_surface(const uintptr_t surface, 
		const struct platform_surface_blit_desc *surface_blit_desc, 
		const uintptr_t window)
{
	SDL_Rect source_rect = {
		.x = surface_blit_desc->source.x,
		.y = surface_blit_desc->source.y,
		.w = surface_blit_desc->source.width,
		.h = surface_blit_desc->source.height
	};
	SDL_Rect dest_rect = {
		.x = surface_blit_desc->destination.x,
		.y = surface_blit_desc->destination.y,
		.w = surface_blit_desc->destination.width,
		.h = surface_blit_desc->destination.height
	};

	/* TODO: Check if blit desc has valid values. */
	SDL_BlitSurface((SDL_Surface *)surface, &source_rect,
			SDL_GetWindowSurface((SDL_Window *)window), &dest_rect);
	SDL_UpdateWindowSurface((SDL_Window *)window);
	/* TODO: Check if BlitSurface failed. */
}

struct platform_interface platform_sdl_make_interface(void)
{
	return (struct platform_interface) {
		.create_backend = create_sdl_backend,
		.destroy_backend = destroy_sdl_backend,
		.poll_event = poll_sdl_event,
		.create_window = create_sdl_window,
		.destroy_window = destroy_sdl_window,
		.create_surface = create_sdl_surface,
		.destroy_surface = destroy_sdl_surface,
		.blit_surface = blit_sdl_surface
	};
}
