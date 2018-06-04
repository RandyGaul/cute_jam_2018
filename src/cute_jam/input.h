int key_once(int key);
int key_down(int key);

int mouse_once(int button);
int mouse_down(int button);

struct MouseInput
{
	int left_button;
	int right_button;
	int wheel_button;
	int wheel_motion;
	int x;
	int y;
	int xrel;
	int yrel;
	int click_type; // 1 - single click; 2 - double click
};

enum
{
	MOUSE_LEFT_BUTTON,
	MOUSE_RIGHT_BUTTON,
	MOUSE_WHEEL_BUTTON
};

MouseInput g_mouse;
MouseInput g_mouse_prev;
int g_cam_xrel_sum;
int g_cam_yrel_sum;

enum
{
	KEY_UNKNOWN = 0,

	KEY_RETURN = '\r',
	KEY_ESCAPE = '\033',
	KEY_BACKSPACE = '\b',
	KEY_TAB = '\t',
	KEY_SPACE = ' ',
	KEY_EXCLAIM = '!',
	KEY_QUOTEDBL = '"',
	KEY_HASH = '#',
	KEY_PERCENT = '%',
	KEY_DOLLAR = '$',
	KEY_AMPERSAND = '&',
	KEY_QUOTE = '\'',
	KEY_LEFTPAREN = '(',
	KEY_RIGHTPAREN = ')',
	KEY_ASTERISK = '*',
	KEY_PLUS = '+',
	KEY_COMMA = ',',
	KEY_MINUS = '-',
	KEY_PERIOD = '.',
	KEY_SLASH = '/',
	KEY_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_COLON = ':',
	KEY_SEMICOLON = ';',
	KEY_LESS = '<',
	KEY_EQUALS = '=',
	KEY_GREATER = '>',
	KEY_QUESTION = '?',
	KEY_AT = '@',
	/*
		Skip uppercase letters
		*/
	KEY_LEFTBRACKET = '[',
	KEY_BACKSLASH = '\\',
	KEY_RIGHTBRACKET = ']',
	KEY_CARET = '^',
	KEY_UNDERSCORE = '_',
	KEY_BACKQUOTE = '`',
	KEY_A = 'a',
	KEY_B = 'b',
	KEY_C = 'c',
	KEY_D = 'd',
	KEY_E = 'e',
	KEY_F = 'f',
	KEY_G = 'g',
	KEY_H = 'h',
	KEY_I = 'i',
	KEY_J = 'j',
	KEY_K = 'k',
	KEY_L = 'l',
	KEY_M = 'm',
	KEY_N = 'n',
	KEY_O = 'o',
	KEY_P = 'p',
	KEY_Q = 'q',
	KEY_R = 'r',
	KEY_S = 's',
	KEY_T = 't',
	KEY_U = 'u',
	KEY_V = 'v',
	KEY_W = 'w',
	KEY_X = 'x',
	KEY_Y = 'y',
	KEY_Z = 'z',

	KEY_CAPSLOCK,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_PRINTSCREEN,
	KEY_SCROLLLOCK,
	KEY_PAUSE,
	KEY_INSERT,
	KEY_HOME,
	KEY_PAGEUP,
	KEY_END,
	KEY_PAGEDOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_DOWN,
	KEY_UP,
	KEY_NUMLOCKCLEAR,
	KEY_KP_DIVIDE,
	KEY_KP_MULTIPLY,
	KEY_KP_MINUS,
	KEY_KP_PLUS,
	KEY_KP_ENTER,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
	KEY_KP_0,
	KEY_KP_PERIOD,
	KEY_APPLICATION,
	KEY_POWER,
	KEY_KP_EQUALS,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,
	KEY_F17,
	KEY_F18,
	KEY_F19,
	KEY_F20,
	KEY_F21,
	KEY_F22,
	KEY_F23,
	KEY_F24,
	KEY_HELP,
	KEY_MENU,
	KEY_SELECT,
	KEY_STOP,
	KEY_AGAIN,
	KEY_UNDO,
	KEY_CUT,
	KEY_COPY,
	KEY_PASTE,
	KEY_FIND,
	KEY_MUTE,
	KEY_VOLUMEUP,
	KEY_VOLUMEDOWN,
	KEY_KP_COMMA,
	KEY_KP_EQUALSAS400,
	KEY_ALTERASE,
	KEY_SYSREQ,
	KEY_CANCEL,
	KEY_CLEAR,
	KEY_PRIOR,
	KEY_RETURN2,
	KEY_SEPARATOR,
	KEY_OUT,
	KEY_OPER,
	KEY_CLEARAGAIN,
	KEY_CRSEL,
	KEY_EXSEL,
	KEY_KP_00,
	KEY_KP_000,
	KEY_THOUSANDSSEPARATOR,
	KEY_DECIMALSEPARATOR,
	KEY_CURRENCYUNIT,
	KEY_CURRENCYSUBUNIT,
	KEY_KP_LEFTPAREN,
	KEY_KP_RIGHTPAREN,
	KEY_KP_LEFTBRACE,
	KEY_KP_RIGHTBRACE,
	KEY_KP_TAB,
	KEY_KP_BACKSPACE,
	KEY_KP_A,
	KEY_KP_B,
	KEY_KP_C,
	KEY_KP_D,
	KEY_KP_E,
	KEY_KP_F,
	KEY_KP_XOR,
	KEY_KP_POWER,
	KEY_KP_PERCENT,
	KEY_KP_LESS,
	KEY_KP_GREATER,
	KEY_KP_AMPERSAND,
	KEY_KP_DBLAMPERSAND,
	KEY_KP_VERTICALBAR,
	KEY_KP_DBLVERTICALBAR,
	KEY_KP_COLON,
	KEY_KP_HASH,
	KEY_KP_SPACE,
	KEY_KP_AT,
	KEY_KP_EXCLAM,
	KEY_KP_MEMSTORE,
	KEY_KP_MEMRECALL,
	KEY_KP_MEMCLEAR,
	KEY_KP_MEMADD,
	KEY_KP_MEMSUBTRACT,
	KEY_KP_MEMMULTIPLY,
	KEY_KP_MEMDIVIDE,
	KEY_KP_PLUSMINUS,
	KEY_KP_CLEAR,
	KEY_KP_CLEARENTRY,
	KEY_KP_BINARY,
	KEY_KP_OCTAL,
	KEY_KP_DECIMAL,
	KEY_KP_HEXADECIMAL,
	KEY_LCTRL,
	KEY_LSHIFT,
	KEY_LALT,
	KEY_LGUI,
	KEY_RCTRL,
	KEY_RSHIFT,
	KEY_RALT,
	KEY_RGUI,
	KEY_MODE,
	KEY_AUDIONEXT,
	KEY_AUDIOPREV,
	KEY_AUDIOSTOP,
	KEY_AUDIOPLAY,
	KEY_AUDIOMUTE,
	KEY_MEDIASELECT,
	KEY_WWW,
	KEY_MAIL,
	KEY_CALCULATOR,
	KEY_COMPUTER,
	KEY_AC_SEARCH,
	KEY_AC_HOME,
	KEY_AC_BACK,
	KEY_AC_FORWARD,
	KEY_AC_STOP,
	KEY_AC_REFRESH,
	KEY_AC_BOOKMARKS,
	KEY_BRIGHTNESSDOWN,
	KEY_BRIGHTNESSUP,
	KEY_DISPLAYSWITCH,
	KEY_KBDILLUMTOGGLE,
	KEY_KBDILLUMDOWN,
	KEY_KBDILLUMUP,
	KEY_EJECT,
	KEY_SLEEP
};

int map_SDL_keys(int key);

int application_running = 1;
void pump_SDL_messages();

//--------------------------------------------------------------------------------------------------

int g_keys[512];
int g_keys_prev[512];

int map_SDL_keys(int key)
{
	if (key < 128) return key;
	switch (key)
	{
		case SDLK_CAPSLOCK: return KEY_CAPSLOCK;
		case SDLK_F1: return KEY_F1;
		case SDLK_F2: return KEY_F2;
		case SDLK_F3: return KEY_F3;
		case SDLK_F4: return KEY_F4;
		case SDLK_F5: return KEY_F5;
		case SDLK_F6: return KEY_F6;
		case SDLK_F7: return KEY_F7;
		case SDLK_F8: return KEY_F8;
		case SDLK_F9: return KEY_F9;
		case SDLK_F10: return KEY_F10;
		case SDLK_F11: return KEY_F11;
		case SDLK_F12: return KEY_F12;
		case SDLK_PRINTSCREEN: return KEY_PRINTSCREEN;
		case SDLK_SCROLLLOCK: return KEY_SCROLLLOCK;
		case SDLK_PAUSE: return KEY_PAUSE;
		case SDLK_INSERT: return KEY_INSERT;
		case SDLK_HOME: return KEY_HOME;
		case SDLK_PAGEUP: return KEY_PAGEUP;
		case SDLK_END: return KEY_END;
		case SDLK_PAGEDOWN: return KEY_PAGEDOWN;
		case SDLK_RIGHT: return KEY_RIGHT;
		case SDLK_LEFT: return KEY_LEFT;
		case SDLK_DOWN: return KEY_DOWN;
		case SDLK_UP: return KEY_UP;
		case SDLK_NUMLOCKCLEAR: return KEY_NUMLOCKCLEAR;
		case SDLK_KP_DIVIDE: return KEY_KP_DIVIDE;
		case SDLK_KP_MULTIPLY: return KEY_KP_MULTIPLY;
		case SDLK_KP_MINUS: return KEY_KP_MINUS;
		case SDLK_KP_PLUS: return KEY_KP_PLUS;
		case SDLK_KP_ENTER: return KEY_KP_ENTER;
		case SDLK_KP_1: return KEY_KP_1;
		case SDLK_KP_2: return KEY_KP_2;
		case SDLK_KP_3: return KEY_KP_3;
		case SDLK_KP_4: return KEY_KP_4;
		case SDLK_KP_5: return KEY_KP_5;
		case SDLK_KP_6: return KEY_KP_6;
		case SDLK_KP_7: return KEY_KP_7;
		case SDLK_KP_8: return KEY_KP_8;
		case SDLK_KP_9: return KEY_KP_9;
		case SDLK_KP_0: return KEY_KP_0;
		case SDLK_KP_PERIOD: return KEY_KP_PERIOD;
		case SDLK_APPLICATION: return KEY_APPLICATION;
		case SDLK_POWER: return KEY_POWER;
		case SDLK_KP_EQUALS: return KEY_KP_EQUALS;
		case SDLK_F13: return KEY_F13;
		case SDLK_F14: return KEY_F14;
		case SDLK_F15: return KEY_F15;
		case SDLK_F16: return KEY_F16;
		case SDLK_F17: return KEY_F17;
		case SDLK_F18: return KEY_F18;
		case SDLK_F19: return KEY_F19;
		case SDLK_F20: return KEY_F20;
		case SDLK_F21: return KEY_F21;
		case SDLK_F22: return KEY_F22;
		case SDLK_F23: return KEY_F23;
		case SDLK_F24: return KEY_F24;
		case SDLK_HELP: return KEY_HELP;
		case SDLK_MENU: return KEY_MENU;
		case SDLK_SELECT: return KEY_SELECT;
		case SDLK_STOP: return KEY_STOP;
		case SDLK_AGAIN: return KEY_AGAIN;
		case SDLK_UNDO: return KEY_UNDO;
		case SDLK_CUT: return KEY_CUT;
		case SDLK_COPY: return KEY_COPY;
		case SDLK_PASTE: return KEY_PASTE;
		case SDLK_FIND: return KEY_FIND;
		case SDLK_MUTE: return KEY_MUTE;
		case SDLK_VOLUMEUP: return KEY_VOLUMEUP;
		case SDLK_VOLUMEDOWN: return KEY_VOLUMEDOWN;
		case SDLK_KP_COMMA: return KEY_KP_COMMA;
		case SDLK_KP_EQUALSAS400: return KEY_KP_EQUALSAS400;
		case SDLK_ALTERASE: return KEY_ALTERASE;
		case SDLK_SYSREQ: return KEY_SYSREQ;
		case SDLK_CANCEL: return KEY_CANCEL;
		case SDLK_CLEAR: return KEY_CLEAR;
		case SDLK_PRIOR: return KEY_PRIOR;
		case SDLK_RETURN2: return KEY_RETURN2;
		case SDLK_SEPARATOR: return KEY_SEPARATOR;
		case SDLK_OUT: return KEY_OUT;
		case SDLK_OPER: return KEY_OPER;
		case SDLK_CLEARAGAIN: return KEY_CLEARAGAIN;
		case SDLK_CRSEL: return KEY_CRSEL;
		case SDLK_EXSEL: return KEY_EXSEL;
		case SDLK_KP_00: return KEY_KP_00;
		case SDLK_KP_000: return KEY_KP_000;
		case SDLK_THOUSANDSSEPARATOR: return KEY_THOUSANDSSEPARATOR;
		case SDLK_DECIMALSEPARATOR: return KEY_DECIMALSEPARATOR;
		case SDLK_CURRENCYUNIT: return KEY_CURRENCYUNIT;
		case SDLK_CURRENCYSUBUNIT: return KEY_CURRENCYSUBUNIT;
		case SDLK_KP_LEFTPAREN: return KEY_KP_LEFTPAREN;
		case SDLK_KP_RIGHTPAREN: return KEY_KP_RIGHTPAREN;
		case SDLK_KP_LEFTBRACE: return KEY_KP_LEFTBRACE;
		case SDLK_KP_RIGHTBRACE: return KEY_KP_RIGHTBRACE;
		case SDLK_KP_TAB: return KEY_KP_TAB;
		case SDLK_KP_BACKSPACE: return KEY_KP_BACKSPACE;
		case SDLK_KP_A: return KEY_KP_A;
		case SDLK_KP_B: return KEY_KP_B;
		case SDLK_KP_C: return KEY_KP_C;
		case SDLK_KP_D: return KEY_KP_D;
		case SDLK_KP_E: return KEY_KP_E;
		case SDLK_KP_F: return KEY_KP_F;
		case SDLK_KP_XOR: return KEY_KP_XOR;
		case SDLK_KP_POWER: return KEY_KP_POWER;
		case SDLK_KP_PERCENT: return KEY_KP_PERCENT;
		case SDLK_KP_LESS: return KEY_KP_LESS;
		case SDLK_KP_GREATER: return KEY_KP_GREATER;
		case SDLK_KP_AMPERSAND: return KEY_KP_AMPERSAND;
		case SDLK_KP_DBLAMPERSAND: return KEY_KP_DBLAMPERSAND;
		case SDLK_KP_VERTICALBAR: return KEY_KP_VERTICALBAR;
		case SDLK_KP_DBLVERTICALBAR: return KEY_KP_DBLVERTICALBAR;
		case SDLK_KP_COLON: return KEY_KP_COLON;
		case SDLK_KP_HASH: return KEY_KP_HASH;
		case SDLK_KP_SPACE: return KEY_KP_SPACE;
		case SDLK_KP_AT: return KEY_KP_AT;
		case SDLK_KP_EXCLAM: return KEY_KP_EXCLAM;
		case SDLK_KP_MEMSTORE: return KEY_KP_MEMSTORE;
		case SDLK_KP_MEMRECALL: return KEY_KP_MEMRECALL;
		case SDLK_KP_MEMCLEAR: return KEY_KP_MEMCLEAR;
		case SDLK_KP_MEMADD: return KEY_KP_MEMADD;
		case SDLK_KP_MEMSUBTRACT: return KEY_KP_MEMSUBTRACT;
		case SDLK_KP_MEMMULTIPLY: return KEY_KP_MEMMULTIPLY;
		case SDLK_KP_MEMDIVIDE: return KEY_KP_MEMDIVIDE;
		case SDLK_KP_PLUSMINUS: return KEY_KP_PLUSMINUS;
		case SDLK_KP_CLEAR: return KEY_KP_CLEAR;
		case SDLK_KP_CLEARENTRY: return KEY_KP_CLEARENTRY;
		case SDLK_KP_BINARY: return KEY_KP_BINARY;
		case SDLK_KP_OCTAL: return KEY_KP_OCTAL;
		case SDLK_KP_DECIMAL: return KEY_KP_DECIMAL;
		case SDLK_KP_HEXADECIMAL: return KEY_KP_HEXADECIMAL;
		case SDLK_LCTRL: return KEY_LCTRL;
		case SDLK_LSHIFT: return KEY_LSHIFT;
		case SDLK_LALT: return KEY_LALT;
		case SDLK_LGUI: return KEY_LGUI;
		case SDLK_RCTRL: return KEY_RCTRL;
		case SDLK_RSHIFT: return KEY_RSHIFT;
		case SDLK_RALT: return KEY_RALT;
		case SDLK_RGUI: return KEY_RGUI;
		case SDLK_MODE: return KEY_MODE;
		case SDLK_AUDIONEXT: return KEY_AUDIONEXT;
		case SDLK_AUDIOPREV: return KEY_AUDIOPREV;
		case SDLK_AUDIOSTOP: return KEY_AUDIOSTOP;
		case SDLK_AUDIOPLAY: return KEY_AUDIOPLAY;
		case SDLK_AUDIOMUTE: return KEY_AUDIOMUTE;
		case SDLK_MEDIASELECT: return KEY_MEDIASELECT;
		case SDLK_WWW: return KEY_WWW;
		case SDLK_MAIL: return KEY_MAIL;
		case SDLK_CALCULATOR: return KEY_CALCULATOR;
		case SDLK_COMPUTER: return KEY_COMPUTER;
		case SDLK_AC_SEARCH: return KEY_AC_SEARCH;
		case SDLK_AC_HOME: return KEY_AC_HOME;
		case SDLK_AC_BACK: return KEY_AC_BACK;
		case SDLK_AC_FORWARD: return KEY_AC_FORWARD;
		case SDLK_AC_STOP: return KEY_AC_STOP;
		case SDLK_AC_REFRESH: return KEY_AC_REFRESH;
		case SDLK_AC_BOOKMARKS: return KEY_AC_BOOKMARKS;
		case SDLK_BRIGHTNESSDOWN: return KEY_BRIGHTNESSDOWN;
		case SDLK_BRIGHTNESSUP: return KEY_BRIGHTNESSUP;
		case SDLK_DISPLAYSWITCH: return KEY_DISPLAYSWITCH;
		case SDLK_KBDILLUMTOGGLE: return KEY_KBDILLUMTOGGLE;
		case SDLK_KBDILLUMDOWN: return KEY_KBDILLUMDOWN;
		case SDLK_KBDILLUMUP: return KEY_KBDILLUMUP;
		case SDLK_EJECT: return KEY_EJECT;
		case SDLK_SLEEP: return KEY_SLEEP;
	}
	return 0;
}

int key_once(int key)
{
	assert(key >= 0 && key < 512);
	return g_keys[key] & !g_keys_prev[key];
}

int key_down(int key)
{
	assert(key >= 0 && key < 512);
	return g_keys[key] | g_keys_prev[key];
}

int mouse_once(int button)
{
	switch (button)
	{
	case MOUSE_LEFT_BUTTON: return g_mouse.left_button & !g_mouse_prev.left_button;
	case MOUSE_RIGHT_BUTTON: return g_mouse.right_button & !g_mouse_prev.right_button;
	case MOUSE_WHEEL_BUTTON: return g_mouse.wheel_button & !g_mouse_prev.wheel_button;
	}
	return 0;
}

int mouse_down(int button)
{
	switch (button)
	{
	case MOUSE_LEFT_BUTTON: return g_mouse.left_button | g_mouse_prev.left_button;
	case MOUSE_RIGHT_BUTTON: return g_mouse.right_button | g_mouse_prev.right_button;
	case MOUSE_WHEEL_BUTTON: return g_mouse.wheel_button | g_mouse_prev.wheel_button;
	}
	return 0;
}

void pump_SDL_messages()
{
	// handle SDL messages
	memcpy(g_keys_prev, g_keys, sizeof(g_keys));
	memcpy(&g_mouse_prev, &g_mouse, sizeof(g_mouse));
	g_mouse.wheel_motion = 0;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			application_running = 0;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				printf("Got a window resize event: %f %f\n", (float)event.window.data1, (float)event.window.data2);
				//Reshape(event.window.data1, event.window.data2);
				break;
			}
			break;

		case SDL_KEYDOWN:
		{
			if (event.key.repeat) continue;
			int key = event.key.keysym.sym;
			key = map_SDL_keys(key);
			assert(key >= 0 && key < 512);
			g_keys[key] = 1;
		}	break;

		case SDL_KEYUP:
		{
			if (event.key.repeat) continue;
			int key = event.key.keysym.sym;
			key = map_SDL_keys(key);
			assert(key >= 0 && key < 512);
			g_keys[key] = 0;
		}	break;

		case SDL_MOUSEMOTION:
			g_mouse.x = event.motion.x;
			g_mouse.y = event.motion.y;
			g_mouse.xrel = event.motion.xrel;
			g_mouse.yrel = -event.motion.yrel;
			g_cam_xrel_sum += g_mouse.xrel;
			g_cam_yrel_sum += g_mouse.yrel;
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT: g_mouse.left_button = 1;
			case SDL_BUTTON_RIGHT: g_mouse.right_button = 1;
			case SDL_BUTTON_MIDDLE: g_mouse.wheel_button = 1;
			}
			g_mouse.x = event.button.x;
			g_mouse.y = event.button.y;
			break;

		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT: g_mouse.left_button = 0;
			case SDL_BUTTON_RIGHT: g_mouse.right_button = 0;
			case SDL_BUTTON_MIDDLE: g_mouse.wheel_button = 0;
			}
			g_mouse.x = event.button.x;
			g_mouse.y = event.button.y;
			break;

		case SDL_MOUSEWHEEL:
			g_mouse.wheel_motion = event.wheel.y;
			break;
		}
	}
}
