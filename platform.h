#ifndef PLATFORM_H
#define PLATFORM_H

struct platform_window_desc {
	int width;
	int height;
	const char *title;
};

struct platform_window {
	int width;
	int height;
	const char *title;
	struct platform *parent_platform;
	void *native_handle;
};

typedef void (*platform_init_fn)(struct platform *, struct platform_desc *);
typedef void (*platform_shutdown_fn)(struct platform *);
typedef void *(*platform_init_window_fn)(struct platform_window_desc *);
typedef void (*platform_finish_window_fn)(struct platform_window *);

struct platform {
	platform_init_fn init;
	platform_shutdown_fn shutdown;
	platform_init_window_fn init_window;
	platform_finish_window_fn finish_window;
	void *native_handle;
};

static inline void platform_init(struct platform *platform, 
		struct platform_desc *platform_desc)
{
	if (platform && platform->init) {
		platform->init(platform, platform_desc);
	}	
}

static inline void platform_shutdown(struct platform *platform)
{
	if (platform && platform->shutdown) {
		platform->shutdown(platform);
	}
}

static inline void platform_window_init(struct platform_window *window,
		struct platform_window_desc *window_desc, 
		struct platform *parent_platform)
{
	if (window && parent_platform && parent_platform->init_window) {
		parent_platform->init_window(parent_platform, window, window_desc);
	}
}

static inline void platform_window_finish(struct platform_window *window)
{
	if (window && window->parent_platform) {
		window->parent_platform->finish_window(window->parent_platform, window);
	}
}

#endif
