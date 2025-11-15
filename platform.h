#ifndef PLATFORM_H
#define PLATFORM_H
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PLATFORM_NATIVE_HANDLE_INVALID ((uintptr_t)0)

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

struct platform_native_override {
	void *handle;
	size_t size;
};

struct platform_window_desc {
	const char *title;
	int32_t width;
	int32_t height;
	uint32_t feature_flags;
	struct platform_native_override *override;
};

struct platform_window {
	uintptr_t native_handle;
	struct platform *parent_platform;
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
	void *pixel_data;
};

struct platform_surface {
	int32_t width;
	int32_t height;
	int32_t pitch;
	void *pixel_data;
	uintptr_t native_handle;
	struct platform *parent_platform;
};

struct platform_desc {
	struct platform_native_override *override;
};

struct platform {
	void (*start)(struct platform *, struct platform_desc *);
	void (*shutdown)(struct platform *);
	bool (*poll_event)(struct platform *, struct platform_event *);
	void (*init_window)(struct platform_window_handle *, struct platform_window_desc *);
	void (*finish_window)(struct platform_window_handle *);
	void (*init_surface)(struct platform_surface_handle *, struct platform_surface_desc *);
	void (*finish_surface)(struct platform_surface_handle *);
	uintptr_t native_handle;
};

static inline void platform_start(struct platform *platform, struct platform_desc *desc)
{
	if (platform && platform->start) {
		platform->start(platform, desc);
	}	
}

static inline void platform_shutdown(struct platform *platform)
{
	if (platform && platform->shutdown) {
		platform->shutdown(platform);
	}
}

static inline bool platform_poll_event(struct platform *platform, struct platform_event *event_out)
{
	if (platform && platform->poll_event && event_out) {
		return platform->poll_event(platform, event_out);
	}
	return false;
}

static inline void platform_window_handle_init(struct platform_window_handle *window,
		struct platform_window_desc *desc, struct platform *parent_platform)
{
	if (window && parent_platform && parent_platform->init_window) {
		window->parent_platform = parent_platform;
		parent_platform->init_window(window, desc);
	}
}

static inline void platform_window_handle_finish(struct platform_window_handle *window)
{
	if (window && window->parent_platform) {
		window->parent_platform->finish_window(window);
	}
}

static inline void platform_surface_handle_init(struct platform_surface_handle *surface,
		struct platform_surface_desc *desc, struct platform *parent_platform)
{
	if (surface && parent_platform && parent_platform->init_surface) {
		parent_platform->init_surface(surface, desc);
	}
}

static inline void platform_surface_handle_finish(struct platform_surface_handle *surface)
{
	if (surface && surface->parent_platform && surface->parent_platform->finish_surface) {
		surface->parent_platform->finish_surface(surface);
	}
}

#endif
