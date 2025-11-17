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
	uintptr_t native_handle;
	const struct platform *parent_platform;
};

struct platform_desc {
	struct platform_native_override *override;
};

struct platform {
	uintptr_t (*start)(const struct platform_desc *);
	uintptr_t (*shutdown)(const uintptr_t);
	bool (*poll_event)(const uintptr_t, struct platform_event *);
	uintptr_t (*create_window)(const struct platform_window_desc *, 
			const uintptr_t);
	void (*destroy_window)(const uintptr_t);
	uintptr_t (*create_surface)(const struct platform_surface_desc *, 
			const uintptr_t);
	void (*destroy_surface)(const uintptr_t);
	void (*blit_surface)(const uintptr_t, 
			const struct platform_surface_blit_desc *, 
			const uintptr_t);
	uintptr_t native_handle;
};

static inline void platform_start(struct platform *platform,
		const struct platform_desc *desc)
{
	assert(platform);
	assert(platform->start);
	platform->native_handle = platform->start(desc);
}

static inline void platform_shutdown(struct platform *platform)
{
	assert(platform);
	assert(platform->shutdown);
	platform->shutdown(platform->native_handle);
	platform->native_handle = PLATFORM_NATIVE_HANDLE_INVALID;
}

static inline bool platform_poll_event(const struct platform *platform, 
		struct platform_event *event_out)
{
	assert(platform);
	assert(platform->poll_event);
	assert(event_out);
	return platform->poll_event(platform->native_handle, event_out);
}

static inline void platform_window_init(struct platform_window *window,
		const struct platform_window_desc *desc, 
		const struct platform *platform)
{
	uintptr_t handle = PLATFORM_NATIVE_HANDLE_INVALID;

	assert(window);
	assert(desc);
	assert(platform);
	handle = platform->create_window(desc, platform->native_handle);
	if (handle == PLATFORM_NATIVE_HANDLE_INVALID) {
		return;
	}
	window->native_handle = handle;
	window->parent_platform = platform;
}

static inline void platform_window_finish(struct platform_window *window)
{
	assert(window);
	assert(window->parent_platform);
	assert(window->parent_platform->destroy_window);
	window->parent_platform->destroy_window(window->native_handle);
	window->native_handle = PLATFORM_NATIVE_HANDLE_INVALID;
	window->parent_platform = NULL;
}

static inline void platform_surface_init(struct platform_surface *surface,
		const struct platform_surface_desc *desc, 
		const struct platform *platform)
{
	uintptr_t handle = PLATFORM_NATIVE_HANDLE_INVALID; 

	assert(surface);
	assert(desc);
	assert(platform);
	handle = platform->create_surface(desc, platform->native_handle);
	if (handle == PLATFORM_NATIVE_HANDLE_INVALID) {
		return;
	}
	surface->width = desc->width;
	surface->height = desc->height;
	surface->pixel_buffer = desc->pixel_buffer;
	surface->native_handle = handle;
	surface->parent_platform = platform;
}

static inline void platform_surface_finish(struct platform_surface *surface)
{
	assert(surface);
	assert(surface->parent_platform);
	assert(surface->parent_platform->destroy_surface);
	surface->parent_platform->destroy_surface(surface->native_handle);
	surface->native_handle = PLATFORM_NATIVE_HANDLE_INVALID;
	surface->parent_platform = NULL;
}

static inline void platform_surface_blit(const struct platform_surface *surface, 
		const struct platform_surface_blit_desc *surface_blit_desc, 
		const struct platform_window *window)
{
	assert(surface);
	assert(surface->parent_platform);
	assert(surface->parent_platform->blit_surface);
	assert(surface_blit_desc);
	assert(window);
	surface->parent_platform->blit_surface(surface->native_handle, 
			surface_blit_desc, window->native_handle);
}

#endif
