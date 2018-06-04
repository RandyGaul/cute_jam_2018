#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#define CUTE_SOUND_IMPLEMENTATION
#include <cute/cute_sound.h>

#define CUTE_SYNC_IMPLEMENTATION
#define CUTE_SYNC_SDL
#include <cute/cute_sync.h>

#define CUTE_ALLOC_IMPLEMENTATION
#include <cute/cute_alloc.h>

#include <launcher.h>
#include <wrap_SDL.h>

// Must happen in main-thread due to DirectSound behaving strangely during
// dll hotloads.
void setup_cute_sound(launcher_t* launcher)
{
	launcher->ctx_cute_sound = cs_make_context(launcher->win32_handle, 44100, 15, 5, 1024);
	cs_spawn_mix_thread(launcher->ctx_cute_sound);
}

// Needs to be kept in launcher to prevent dll hotloading from wiping any
// synchronization primitives.
void setup_threadpool(launcher_t* launcher)
{
	launcher->threadpool = cute_threadpool_create(cute_core_count() * 2, 0);
}

void shutdown_cute_sound(launcher_t* launcher)
{
	cs_shutdown_context(launcher->ctx_cute_sound);
}

void shutdown_threadpool(launcher_t* launcher)
{
	cute_threadpool_destroy(launcher->threadpool);
}

#if 1
void* launcher_alloc(size_t size, const char* file, int line)
{
	(void)file;
	(void)line;
	return ca_leak_check_alloc(size, file, line);
}

void launcher_free(void* ptr)
{
	ca_leak_check_free(ptr);
}
#else
void* launcher_alloc(size_t size, const char* file, int line)
{
	(void)file;
	(void)line;
	return malloc(size);
}

void launcher_free(void* ptr)
{
	free(ptr);
}
#endif

#include <crtdbg.h>

int main(int argc, char** argv)
{
	// Setup Windows memory debugging
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(flag);

	launcher_t launcher;
	launcher.alloc_fn = launcher_alloc;
	launcher.free_fn = launcher_free;

	setup_SDL(&launcher, "cute jam", 640, 480);
	setup_cute_sound(&launcher);
	setup_threadpool(&launcher);

	do_hotload(&launcher);
	dll_single_time_initialization(&launcher);

	// main loop
	while (1)
	{
		hotload_if_needed(&launcher);
		int running = dll_main_loop(&launcher);
		if (!running) break;
	}

	dll_shutdown(&launcher);

	shutdown_cute_sound(&launcher);
	shutdown_threadpool(&launcher);

	CUTE_ALLOC_CHECK_FOR_LEAKS();

	return 0;
}
