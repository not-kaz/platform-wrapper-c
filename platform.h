#ifndef PLATFORM_H
#define PLATFORM_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct platform;

enum platform_event_type {
	PLATFORM_EVENT_TYPE_UNDEFINED = 0,
	/* Application events */
	PLATFORM_EVENT_TYPE_QUIT,
	/* Window events */
	PLATFORM_EVENT_TYPE_WINDOW_SHOWN,
	PLATFORM_EVENT_TYPE_WINDOW_HIDDEN,
	PLATFORM_EVENT_TYPE_WINDOW_RESIZED,
	PLATFORM_EVENT_TYPE_WINDOW_MINIMIZED,
	PLATFORM_EVENT_TYPE_WINDOW_MAXIMIZED,
	PLATFORM_EVENT_TYPE_WINDOW_DESTROYED,
	PLATFORM_EVENT_TYPE_WINDOW_FOCUS_GAINED,
	PLATFORM_EVENT_TYPE_WINDOW_FOCUS_LOST,
	/* Keyboard events */
	PLATFORM_EVENT_TYPE_KEY_DOWN,
	PLATFORM_EVENT_TYPE_KEY_UP,
	PLATFORM_EVENT_TYPE_KEYBOARD_ADDED,
	PLATFORM_EVENT_TYPE_KEYBOARD_REMOVED,
	/* Mouse events */
	PLATFORM_EVENT_TYPE_MOUSE_MOTION,
	PLATFORM_EVENT_TYPE_MOUSE_BUTTON_DOWN,
	PLATFORM_EVENT_TYPE_MOUSE_BUTTON_UP,
	PLATFORM_EVENT_TYPE_MOUSE_WHEEL,
	PLATFORM_EVENT_TYPE_MOUSE_ADDED,
	PLATFORM_EVENT_TYPE_MOUSE_REMOVED,
	/* Gamepad events */
	PLATFORM_EVENT_TYPE_GAMEPAD_AXIS_MOTION,
	PLATFORM_EVENT_TYPE_GAMEPAD_BUTTON_DOWN,
	PLATFORM_EVENT_TYPE_GAMEPAD_BUTTON_UP,
	PLATFORM_EVENT_TYPE_GAMEPAD_ADDED,
	PLATFORM_EVENT_TYPE_GAMEPAD_REMOVED,
	/* Clipboard events */
	PLATFORM_EVENT_TYPE_CLIPBOARD_UPDATE,
	/* */
	PLATFORM_EVENT_TYPE_NUMBER_OF
};

enum platform_window_feature {
	PLATFORM_WINDOW_FEATURE_FULLSCREEN = 1u << 0,
	PLATFORM_WINDOW_FEATURE_BORDERLESS = 1u << 1,
	PLATFORM_WINDOW_FEATURE_RESIZABLE = 1u << 2,
	PLATFORM_WINDOW_FEATURE_MINIMIZED = 1u << 3,
	PLATFORM_WINDOW_FEATURE_MAXIMIZED = 1u << 4
};

struct platform_key_event {
	int unused;
};

struct platform_mouse_button_event {
	int unused;
};

struct platform_window_event {
	int unused;
};

struct platform_text_event {
	int unused;
};

struct platform_event {
	enum platform_event_type type;
	union {
		struct platform_key_event key;
		struct platform_mouse_button_event mouse_button;
		struct platform_window_event window;
		struct platform_text_event text;
	} detail;
};

struct platform_window_desc {
	const char *title;
	int32_t width;
	int32_t height;
	uint32_t feature_flags;
};

struct platform_window {
	uintptr_t handle;
	const struct platform *platform;
};

struct platform_surface_blit_desc {
	struct {
		int32_t x;
		int32_t y;
		int32_t width;
		int32_t height;
	} source;
	struct {
		int32_t x;
		int32_t y;
		int32_t width;
		int32_t height;
	} destination;
};

struct platform_surface_desc {
	int32_t width;
	int32_t height;
	void *pixel_buffer;
};

struct platform_surface {
	int32_t width;
	int32_t height;
	int32_t pitch;
	void *pixel_buffer;
	uintptr_t handle;
	const struct platform *platform;
};

struct platform_interface {
	uintptr_t (*create_backend)(void);
	void (*destroy_backend)(const uintptr_t platform_handle);
	bool (*poll_event)(struct platform_event *event_out,
			uintptr_t platform_handle);
	uintptr_t (*create_window)(
			const struct platform_window_desc *window_desc, 
			const uintptr_t platform_handle);
	void (*destroy_window)(const uintptr_t window_handle);
	uintptr_t (*create_surface)(
			const struct platform_surface_desc *surface_desc, 
			const uintptr_t platform_handle);
	void (*destroy_surface)(const uintptr_t surface_handle);
	void (*blit_surface)(const uintptr_t surface_handle, 
			const struct platform_surface_blit_desc *, 
			const uintptr_t window_handle);
};

struct platform {
	struct platform_interface interface;
	uintptr_t handle;
};

static inline void platform_init(struct platform *platform, 
		struct platform_interface platform_interface)
{
	platform->interface = platform_interface;
	platform->handle = platform->interface.create_backend();
}

static inline void platform_finish(struct platform *platform)
{
	platform->interface.destroy_backend(platform->handle);
	platform->handle = (uintptr_t)NULL;
}

static inline bool platform_poll_event(const struct platform *platform, 
		struct platform_event *event_out)
{
	return platform->interface.poll_event(event_out, platform->handle);
}

static inline void platform_window_init(struct platform_window *window,
		const struct platform_window_desc *desc, 
		const struct platform *platform)
{
	window->handle = platform->interface.create_window(desc, platform->handle);
	window->platform = platform;
}

static inline void platform_window_finish(struct platform_window *window)
{
	window->platform->interface.destroy_window(window->handle);
	window->handle = (uintptr_t)NULL;
	window->platform = NULL;
}

static inline void platform_surface_init(struct platform_surface *surface,
		const struct platform_surface_desc *desc, 
		const struct platform *platform)
{
	surface->handle = platform->interface.create_surface(desc, platform->handle);
	surface->platform = platform;
	surface->width = desc->width;
	surface->height = desc->height;
	surface->pixel_buffer = desc->pixel_buffer;
}

static inline void platform_surface_finish(struct platform_surface *surface)
{
	surface->platform->interface.destroy_surface(surface->handle);
	surface->handle = (uintptr_t)NULL;
	surface->platform = NULL;
}

static inline void platform_surface_blit(const struct platform_surface *surface, 
		const struct platform_surface_blit_desc *surface_blit_desc, 
		const struct platform_window *window)
{
	surface->platform->interface.blit_surface(surface->handle, 
			surface_blit_desc, window->handle);
}

#endif
