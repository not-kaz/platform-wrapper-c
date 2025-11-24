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
	/* Number of event types */
	PLATFORM_EVENT_TYPE_NUMBER_OF
};

enum platform_window_feature {
	PLATFORM_WINDOW_FEATURE_FULLSCREEN = 1u << 0,
	PLATFORM_WINDOW_FEATURE_BORDERLESS = 1u << 1,
	PLATFORM_WINDOW_FEATURE_RESIZABLE = 1u << 2,
	PLATFORM_WINDOW_FEATURE_MINIMIZED = 1u << 3,
	PLATFORM_WINDOW_FEATURE_MAXIMIZED = 1u << 4
};

enum platform_key_scancode {
    PLATFORM_KEY_SCANCODE_UNKNOWN = 0,
    /* Alphabetic keys */
    PLATFORM_KEY_SCANCODE_B = 5,
    PLATFORM_KEY_SCANCODE_C = 6,
    PLATFORM_KEY_SCANCODE_D = 7,
    PLATFORM_KEY_SCANCODE_E = 8,
    PLATFORM_KEY_SCANCODE_F = 9,
    PLATFORM_KEY_SCANCODE_G = 10,
    PLATFORM_KEY_SCANCODE_H = 11,
    PLATFORM_KEY_SCANCODE_I = 12,
    PLATFORM_KEY_SCANCODE_J = 13,
    PLATFORM_KEY_SCANCODE_K = 14,
    PLATFORM_KEY_SCANCODE_L = 15,
    PLATFORM_KEY_SCANCODE_M = 16,
    PLATFORM_KEY_SCANCODE_N = 17,
    PLATFORM_KEY_SCANCODE_O = 18,
    PLATFORM_KEY_SCANCODE_P = 19,
    PLATFORM_KEY_SCANCODE_Q = 20,
    PLATFORM_KEY_SCANCODE_R = 21,
    PLATFORM_KEY_SCANCODE_S = 22,
    PLATFORM_KEY_SCANCODE_T = 23,
    PLATFORM_KEY_SCANCODE_U = 24,
    PLATFORM_KEY_SCANCODE_V = 25,
    PLATFORM_KEY_SCANCODE_W = 26,
    PLATFORM_KEY_SCANCODE_X = 27,
    PLATFORM_KEY_SCANCODE_Y = 28,
    PLATFORM_KEY_SCANCODE_Z = 29,
    /* Number row keys */
    PLATFORM_KEY_SCANCODE_1 = 30,
    PLATFORM_KEY_SCANCODE_2 = 31,
    PLATFORM_KEY_SCANCODE_3 = 32,
    PLATFORM_KEY_SCANCODE_4 = 33,
    PLATFORM_KEY_SCANCODE_5 = 34,
    PLATFORM_KEY_SCANCODE_6 = 35,
    PLATFORM_KEY_SCANCODE_7 = 36,
    PLATFORM_KEY_SCANCODE_8 = 37,
    PLATFORM_KEY_SCANCODE_9 = 38,
    PLATFORM_KEY_SCANCODE_0 = 39,
    /* Basic control keys */
    PLATFORM_KEY_SCANCODE_RETURN = 40,
    PLATFORM_KEY_SCANCODE_ESCAPE = 41,
    PLATFORM_KEY_SCANCODE_BACKSPACE = 42,
    PLATFORM_KEY_SCANCODE_TAB = 43,
    PLATFORM_KEY_SCANCODE_SPACE = 44,
    /* Symbol  keys */
    PLATFORM_KEY_SCANCODE_MINUS = 45,
    PLATFORM_KEY_SCANCODE_EQUALS = 46,
    PLATFORM_KEY_SCANCODE_LEFTBRACKET = 47,
    PLATFORM_KEY_SCANCODE_RIGHTBRACKET = 48,
    PLATFORM_KEY_SCANCODE_BACKSLASH = 49,
    PLATFORM_KEY_SCANCODE_NONUSBACKSLASH = 50,
    PLATFORM_KEY_SCANCODE_SEMICOLON = 51,
    PLATFORM_KEY_SCANCODE_APOSTROPHE = 52,
    PLATFORM_KEY_SCANCODE_GRAVE = 53,
    PLATFORM_KEY_SCANCODE_COMMA = 54,
    PLATFORM_KEY_SCANCODE_PERIOD = 55,
    PLATFORM_KEY_SCANCODE_SLASH = 56,
    /* Lock key */
    PLATFORM_KEY_SCANCODE_CAPSLOCK = 57,
    /* Function keys */
    PLATFORM_KEY_SCANCODE_F1 = 58,
    PLATFORM_KEY_SCANCODE_F2 = 59,
    PLATFORM_KEY_SCANCODE_F3 = 60,
    PLATFORM_KEY_SCANCODE_F4 = 61,
    PLATFORM_KEY_SCANCODE_F5 = 62,
    PLATFORM_KEY_SCANCODE_F6 = 63,
    PLATFORM_KEY_SCANCODE_F7 = 64,
    PLATFORM_KEY_SCANCODE_F8 = 65,
    PLATFORM_KEY_SCANCODE_F9 = 66,
    PLATFORM_KEY_SCANCODE_F10 = 67,
    PLATFORM_KEY_SCANCODE_F11 = 68,
    PLATFORM_KEY_SCANCODE_F12 = 69,
    /* System keys */
    PLATFORM_KEY_SCANCODE_PRINTSCREEN = 70,
    PLATFORM_KEY_SCANCODE_SCROLLLOCK = 71,
    PLATFORM_KEY_SCANCODE_PAUSE = 72,
    PLATFORM_KEY_SCANCODE_INSERT = 73,
    PLATFORM_KEY_SCANCODE_HOME = 74,
    PLATFORM_KEY_SCANCODE_PAGEUP = 75,
    PLATFORM_KEY_SCANCODE_DELETE = 76,
    PLATFORM_KEY_SCANCODE_END = 77,
    PLATFORM_KEY_SCANCODE_PAGEDOWN = 78,
    PLATFORM_KEY_SCANCODE_RIGHT = 79,
    PLATFORM_KEY_SCANCODE_LEFT = 80,
    PLATFORM_KEY_SCANCODE_DOWN = 81,
    PLATFORM_KEY_SCANCODE_UP = 82,
    /* Numeric keypad keys */
    PLATFORM_KEY_SCANCODE_NUMLOCKCLEAR = 83,
    PLATFORM_KEY_SCANCODE_KP_DIVIDE = 84,
    PLATFORM_KEY_SCANCODE_KP_MULTIPLY = 85,
    PLATFORM_KEY_SCANCODE_KP_MINUS = 86,
    PLATFORM_KEY_SCANCODE_KP_PLUS = 87,
    PLATFORM_KEY_SCANCODE_KP_ENTER = 88,
    PLATFORM_KEY_SCANCODE_KP_1 = 89,
    PLATFORM_KEY_SCANCODE_KP_2 = 90,
    PLATFORM_KEY_SCANCODE_KP_3 = 91,
    PLATFORM_KEY_SCANCODE_KP_4 = 92,
    PLATFORM_KEY_SCANCODE_KP_5 = 93,
    PLATFORM_KEY_SCANCODE_KP_6 = 94,
    PLATFORM_KEY_SCANCODE_KP_7 = 95,
    PLATFORM_KEY_SCANCODE_KP_8 = 96,
    PLATFORM_KEY_SCANCODE_KP_9 = 97,
    PLATFORM_KEY_SCANCODE_KP_0 = 98,
    PLATFORM_KEY_SCANCODE_KP_PERIOD = 99,
    /* Keypad function keys */
    PLATFORM_KEY_SCANCODE_NONUSBACKSLASH2 = 100,
    PLATFORM_KEY_SCANCODE_APPLICATION = 101,
    PLATFORM_KEY_SCANCODE_POWER = 102,
    PLATFORM_KEY_SCANCODE_KP_EQUALS = 103,
    /* Extra function keys */
    PLATFORM_KEY_SCANCODE_F13 = 104,
    PLATFORM_KEY_SCANCODE_F14 = 105,
    PLATFORM_KEY_SCANCODE_F15 = 106,
    PLATFORM_KEY_SCANCODE_F16 = 107,
    PLATFORM_KEY_SCANCODE_F17 = 108,
    PLATFORM_KEY_SCANCODE_F18 = 109,
    PLATFORM_KEY_SCANCODE_F19 = 110,
    PLATFORM_KEY_SCANCODE_F20 = 111,
    PLATFORM_KEY_SCANCODE_F21 = 112,
    PLATFORM_KEY_SCANCODE_F22 = 113,
    PLATFORM_KEY_SCANCODE_F23 = 114,
    PLATFORM_KEY_SCANCODE_F24 = 115,
    /* Application control keys */
    PLATFORM_KEY_SCANCODE_EXECUTE = 116,
    PLATFORM_KEY_SCANCODE_HELP = 117,
    PLATFORM_KEY_SCANCODE_MENU = 118,
    PLATFORM_KEY_SCANCODE_SELECT = 119,
    PLATFORM_KEY_SCANCODE_STOP = 120,
    PLATFORM_KEY_SCANCODE_AGAIN = 121,
    PLATFORM_KEY_SCANCODE_UNDO = 122,
    PLATFORM_KEY_SCANCODE_CUT = 123,
    PLATFORM_KEY_SCANCODE_COPY = 124,
    PLATFORM_KEY_SCANCODE_PASTE = 125,
    PLATFORM_KEY_SCANCODE_FIND = 126,
    PLATFORM_KEY_SCANCODE_MUTE = 127,
    PLATFORM_KEY_SCANCODE_VOLUMEUP = 128,
    PLATFORM_KEY_SCANCODE_VOLUMEDOWN = 129,
    /* Special keypad extensions */
    PLATFORM_KEY_SCANCODE_KP_COMMA = 133,
    PLATFORM_KEY_SCANCODE_KP_EQUALSAS400 = 134,
    /* International layout keys */
    PLATFORM_KEY_SCANCODE_INTERNATIONAL1 = 135,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL2 = 136,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL3 = 137,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL4 = 138,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL5 = 139,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL6 = 140,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL7 = 141,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL8 = 142,
    PLATFORM_KEY_SCANCODE_INTERNATIONAL9 = 143,
    /* Language switching keys */
    PLATFORM_KEY_SCANCODE_LANG1 = 144,
    PLATFORM_KEY_SCANCODE_LANG2 = 145,
    PLATFORM_KEY_SCANCODE_LANG3 = 146,
    PLATFORM_KEY_SCANCODE_LANG4 = 147,
    PLATFORM_KEY_SCANCODE_LANG5 = 148,
    PLATFORM_KEY_SCANCODE_LANG6 = 149,
    PLATFORM_KEY_SCANCODE_LANG7 = 150,
    PLATFORM_KEY_SCANCODE_LANG8 = 151,
    PLATFORM_KEY_SCANCODE_LANG9 = 152,
    /* Extended system request keys */
    PLATFORM_KEY_SCANCODE_ALTERASE = 153,
    PLATFORM_KEY_SCANCODE_SYSREQ = 154,
    PLATFORM_KEY_SCANCODE_CANCEL = 155,
    PLATFORM_KEY_SCANCODE_CLEAR = 156,
    PLATFORM_KEY_SCANCODE_PRIOR = 157,
    PLATFORM_KEY_SCANCODE_RETURN2 = 158,
    PLATFORM_KEY_SCANCODE_SEPARATOR = 159,
    PLATFORM_KEY_SCANCODE_OUT = 160,
    PLATFORM_KEY_SCANCODE_OPER = 161,
    PLATFORM_KEY_SCANCODE_CLEARAGAIN = 162,
    PLATFORM_KEY_SCANCODE_CRSEL = 163,
    PLATFORM_KEY_SCANCODE_EXSEL = 164,
    /* Advanced keypad keys */
    PLATFORM_KEY_SCANCODE_KP_00 = 176,
    PLATFORM_KEY_SCANCODE_KP_000 = 177,
    PLATFORM_KEY_SCANCODE_THOUSANDSSEPARATOR = 178,
    PLATFORM_KEY_SCANCODE_DECIMALSEPARATOR = 179,
    PLATFORM_KEY_SCANCODE_CURRENCYUNIT = 180,
    PLATFORM_KEY_SCANCODE_CURRENCYSUBUNIT = 181,
    PLATFORM_KEY_SCANCODE_KP_LEFTPAREN = 182,
    PLATFORM_KEY_SCANCODE_KP_RIGHTPAREN = 183,
    PLATFORM_KEY_SCANCODE_KP_LEFTBRACE = 184,
    PLATFORM_KEY_SCANCODE_KP_RIGHTBRACE = 185,
    PLATFORM_KEY_SCANCODE_KP_TAB = 186,
    PLATFORM_KEY_SCANCODE_KP_BACKSPACE = 187,
    PLATFORM_KEY_SCANCODE_KP_A = 188,
    PLATFORM_KEY_SCANCODE_KP_B = 189,
    PLATFORM_KEY_SCANCODE_KP_C = 190,
    PLATFORM_KEY_SCANCODE_KP_D = 191,
    PLATFORM_KEY_SCANCODE_KP_E = 192,
    PLATFORM_KEY_SCANCODE_KP_F = 193,
    PLATFORM_KEY_SCANCODE_KP_XOR = 194,
    PLATFORM_KEY_SCANCODE_KP_POWER = 195,
    PLATFORM_KEY_SCANCODE_KP_PERCENT = 196,
    PLATFORM_KEY_SCANCODE_KP_LESS = 197,
    PLATFORM_KEY_SCANCODE_KP_GREATER = 198,
    PLATFORM_KEY_SCANCODE_KP_AMPERSAND = 199,
    PLATFORM_KEY_SCANCODE_KP_DBLAMPERSAND = 200,
    PLATFORM_KEY_SCANCODE_KP_VERTICALBAR = 201,
    PLATFORM_KEY_SCANCODE_KP_DBLVERTICALBAR = 202,
    PLATFORM_KEY_SCANCODE_KP_COLON = 203,
    PLATFORM_KEY_SCANCODE_KP_HASH = 204,
    PLATFORM_KEY_SCANCODE_KP_SPACE = 205,
    PLATFORM_KEY_SCANCODE_KP_AT = 206,
    PLATFORM_KEY_SCANCODE_KP_EXCLAM = 207,
    PLATFORM_KEY_SCANCODE_KP_MEMSTORE = 208,
    PLATFORM_KEY_SCANCODE_KP_MEMRECALL = 209,
    PLATFORM_KEY_SCANCODE_KP_MEMCLEAR = 210,
    PLATFORM_KEY_SCANCODE_KP_MEMADD = 211,
    PLATFORM_KEY_SCANCODE_KP_MEMSUBTRACT = 212,
    PLATFORM_KEY_SCANCODE_KP_MEMMULTIPLY = 213,
    PLATFORM_KEY_SCANCODE_KP_MEMDIVIDE = 214,
    PLATFORM_KEY_SCANCODE_KP_PLUSMINUS = 215,
    PLATFORM_KEY_SCANCODE_KP_CLEAR = 216,
    PLATFORM_KEY_SCANCODE_KP_CLEARENTRY = 217,
    PLATFORM_KEY_SCANCODE_KP_BINARY = 218,
    PLATFORM_KEY_SCANCODE_KP_OCTAL = 219,
    PLATFORM_KEY_SCANCODE_KP_DECIMAL = 220,
    PLATFORM_KEY_SCANCODE_KP_HEXADECIMAL = 221,
    /* Modifier keys */
    PLATFORM_KEY_SCANCODE_LCTRL = 224,
    PLATFORM_KEY_SCANCODE_LSHIFT = 225,
    PLATFORM_KEY_SCANCODE_LALT = 226,
    PLATFORM_KEY_SCANCODE_LGUI = 227,
    PLATFORM_KEY_SCANCODE_RCTRL = 228,
    PLATFORM_KEY_SCANCODE_RSHIFT = 229,
    PLATFORM_KEY_SCANCODE_RALT = 230,
    PLATFORM_KEY_SCANCODE_RGUI = 231,
    /* Number of keys */
    PLATFORM_KEY_SCANCODE_NUMBER_OF
};


enum platform_key_mod {
	PLATFORM_KEY_MOD_SHIFT = 1u << 0,
	PLATFORM_KEY_MOD_CTRL = 1u << 1,
	PLATFORM_KEY_MOD_ALT = 1u << 2,
	PLATFORM_KEY_MOD_SUPER = 1u << 3
};

struct platform_key_event {
	int unused;
};

struct platform_mouse_button_event {
	int unused;
};

struct platform_mouse_motion_event {
	int unused;
};

struct platform_window_event {
	int unused;
};

struct platform_event {
	enum platform_event_type type;
	union {
		struct platform_key_event key;
		struct platform_mouse_button_event mouse_button;
		struct platform_mouse_motion_event mouse_motion;
		struct platform_window_event window;
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
			const uintptr_t platform_handle);
	uintptr_t (*create_window)(
			const struct platform_window_desc *window_desc, 
			const uintptr_t platform_handle);
	void (*destroy_window)(const uintptr_t window_handle);
	uintptr_t (*create_surface)(
			const struct platform_surface_desc *surface_desc, 
			const uintptr_t platform_handle);
	void (*destroy_surface)(const uintptr_t surface_handle);
	void (*blit_surface)(const uintptr_t surface_handle, 
			const struct platform_surface_blit_desc *blit_desc,
			const uintptr_t window_handle);
};

struct platform {
	struct platform_interface interface;
	uintptr_t handle;
};

static inline bool platform_init(struct platform *platform, 
		const struct platform_interface *platform_interface)
{
	uintptr_t handle;

	handle = platform_interface->create_backend();
	if (handle == (uintptr_t)NULL) {
		return false;
	}
	platform->handle = handle;
	platform->interface = *platform_interface;
	return true;
}

static inline void platform_finish(struct platform *platform)
{
	platform->interface.destroy_backend(platform->handle);
}

static inline bool platform_poll_event(const struct platform *platform, 
		struct platform_event *event_out)
{
	return platform->interface.poll_event(event_out, platform->handle);
}

static inline bool platform_window_init(struct platform_window *window,
		const struct platform_window_desc *desc, 
		const struct platform *platform)
{
	uintptr_t handle;

	handle = platform->interface.create_window(desc, platform->handle);
	if (handle == (uintptr_t)NULL) {
		return false;
	}
	window->handle = handle;
	window->platform = platform;
	return true;
}

static inline void platform_window_finish(struct platform_window *window)
{
	window->platform->interface.destroy_window(window->handle);
}

static inline bool platform_surface_init(struct platform_surface *surface,
		const struct platform_surface_desc *desc, 
		const struct platform *platform)
{
	uintptr_t handle;

	handle = platform->interface.create_surface(desc, platform->handle);
	if (handle == (uintptr_t)NULL) {
		return false;
	}
	surface->handle = handle;
	surface->platform = platform;
	surface->width = desc->width;
	surface->height = desc->height;
	surface->pixel_buffer = desc->pixel_buffer;
	return true;
}

static inline void platform_surface_finish(struct platform_surface *surface)
{
	surface->platform->interface.destroy_surface(surface->handle);
}

static inline void platform_surface_blit(const struct platform_surface *surface, 
		const struct platform_surface_blit_desc *surface_blit_desc, 
		const struct platform_window *window)
{
	surface->platform->interface.blit_surface(surface->handle, 
			surface_blit_desc, window->handle);
}

#endif
