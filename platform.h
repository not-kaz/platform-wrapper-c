#ifndef PLATFORM_H
#define PLATFORM_H
#include <stddef.h>
#include <stdint.h>

struct platform;

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

struct platform_window_handle {
	uintptr_t native_handle;
	struct platform *parent_platform;
};

struct platform_surface_desc {
	int32_t width;
	int32_t height;
	void *pixel_data;
};

struct platform_surface_handle {
	int32_t width;
	int32_t height;
	int32_t pitch;
	void *pixel_data;
	uintptr_t native_handle;
	struct platform_window_handle *parent_window;
};

struct platform_desc {
	struct platform_native_override *override;
};

struct platform {
	void (*start)(struct platform *, struct platform_desc *);
	void (*shutdown)(struct platform *);
	void (*init_window)(struct platform_window_handle *, struct platform_window_desc *);
	void (*finish_window)(struct platform_window_handle *);
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

#endif
