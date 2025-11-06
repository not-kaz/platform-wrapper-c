#ifndef PLATFORM_H
#define PLATFORM_H

enum platform_native_override_policy {
	PLATFORM_NATIVE_OVERRIDE_POLICY_NONE,
	PLATFORM_NATIVE_OVERRIDE_POLICY_AUGMENT,
	PLATFORM_NATIVE_OVERRIDE_POLICY_REPLACE
};

struct platform_native_overrides {
	enum platform_native_override_policy policy;
	int init_flags;
	int shutdown_flags;
};

struct platform {
	void (*init)(struct platform *, struct platform_desc *);
	void (*shutdown)(struct platform *);
	void (*init_window)(struct platform *, struct platform_window *, 
			struct platform_window_desc *);
	void (*finish_window)(struct platform_window *);
	void *native_handle;
};

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
