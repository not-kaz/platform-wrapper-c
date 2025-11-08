#ifndef PLATFORM_H
#define PLATFORM_H
#include <stddef.h>
#include <stdint.h>

struct platform;

struct platform_window_desc {
	int32_t width;
	int32_t height;
	const char *title;
	void *config_handle;
	size_t config_handle_size;
};

struct platform_window_handle {
	struct platform *parent_platform;
	void *native_handle;
};

struct platform_desc {
	void *config_handle;
	size_t config_handle_size;
};

struct platform {
	void (*start)(struct platform *, struct platform_desc *);
	void (*shutdown)(struct platform *);
	void (*init_window)(struct platform *, struct platform_window_handle *, 
			struct platform_window_desc *);
	void (*finish_window)(struct platform_window_handle *);
	void *native_handle;
};

static inline void platform_start(struct platform *platform, 
		struct platform_desc *platform_desc)
{
	if (platform && platform->start) {
		platform->start(platform, platform_desc);
	}	
}

static inline void platform_shutdown(struct platform *platform)
{
	if (platform && platform->shutdown) {
		platform->shutdown(platform);
	}
}

static inline void platform_window_handle_init(struct platform_window_handle *window,
		struct platform_window_desc *window_desc, 
		struct platform *parent_platform)
{
	if (window && parent_platform && parent_platform->init_window) {
		parent_platform->init_window(parent_platform, window, window_desc);
	}
}

static inline void platform_window_handle_finish(struct platform_window_handle *window)
{
	if (window && window->parent_platform) {
		window->parent_platform->finish_window(window);
	}
}

#endif
