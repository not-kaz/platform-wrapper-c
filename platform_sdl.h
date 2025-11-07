#ifndef PLATFORM_SDL_H
#define PLATFORM_SDL_H

struct platform;

struct platform_sdl_config {
	int init_flags;
	void (*exec_pre_start_hook)(struct platform *);
	void (*exec_post_start_hook)(struct platform *);
};

struct platform_sdl_window_config {
	int init_flags;
};

struct platform_sdl_window_extension {
	void *gl_context;
};

void platform_sdl_bind(struct platform *platform);

#endif
