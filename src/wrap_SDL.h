void setup_SDL(launcher_t* launcher, const char* title, int w, int h)
{
	// Setup SDL and OpenGL and a window
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

	// Request OpenGL 3.2 context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm))
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return;
	}

	int screen_w = w;
	int screen_h = h;
	int centered_x = dm.w / 2 - screen_w / 2;
	int centered_y = dm.h / 2 - screen_h / 2;
	launcher->window = SDL_CreateWindow(title, centered_x, centered_y, screen_w, screen_h, SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI);
	launcher->ctx_sdl_gl = SDL_GL_CreateContext(launcher->window);

	// set double buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// immediate swaps
	SDL_GL_SetSwapInterval(0);

	int major, minor;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
	printf("SDL says running on OpenGL ES version %d.%d\n", major, minor);

	SDL_SysWMinfo info;
	SDL_GetWindowWMInfo(launcher->window, &info);
	SDL_VERSION(&info.version);

	if (SDL_GetWindowWMInfo(launcher->window, &info))
	{
		const char *subsystem = "an unknown system!";
		switch(info.subsystem)
		{
			case SDL_SYSWM_UNKNOWN:   break;
			case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows(TM)";  break;
			case SDL_SYSWM_X11:       subsystem = "X Window System";        break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
			case SDL_SYSWM_WINRT:     subsystem = "WinRT";                  break;
#endif
			case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;
			case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";             break;
			case SDL_SYSWM_UIKIT:     subsystem = "UIKit";                  break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
			case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";                break;
			case SDL_SYSWM_MIR:       subsystem = "Mir";                    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
			case SDL_SYSWM_ANDROID:   subsystem = "Android";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
			case SDL_SYSWM_VIVANTE:   subsystem = "Vivante";                break;
#endif
		}

		SDL_Log("This program is running SDL version %d.%d.%d on %s",
			(int)info.version.major,
			(int)info.version.minor,
			(int)info.version.patch,
			subsystem);
	}
	else SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't get window information: %s", SDL_GetError());

	launcher->win32_handle = info.info.win.window;
}
