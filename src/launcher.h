#define __WINDOWS__		1
#define __IOS__			2
#define __MAC__			3
#define __LINUX__		4

#if defined(_MSC_VER)

	#define __PLATFORM__ __WINDOWS__

#elif defined(__APPLE__)

	#include "TargetConditionals.h"

	#if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)

		#define __PLATFORM__ __IOS__

	#elif defined(TARGET_OS_MAC)

		#define __PLATFORM__ __MAC__

	#else

		#error "Unsupported platform."

	#endif

#elif defined(__linux__)

	#define __PLATFORM__ __LINUX__

#endif

typedef void* (global_alloc_t)(size_t size, const char* file, int line);
typedef void (global_free_t)(void* mem);

struct launcher_t
{
	// game globals
	global_alloc_t* alloc_fn;
	global_free_t* free_fn;
	void* ctx_game;
	struct cs_context_t* ctx_cute_sound;
	struct cute_threadpool_t* threadpool;

	// SDL2 globals
	SDL_Window* window;
	SDL_GLContext ctx_sdl_gl;

	// platform specifics
	#if __PLATFORM__ == __WINDOWS__

		HANDLE win32_handle;

	#endif
};

#define EXPORT extern "C" __declspec( dllexport )

typedef int (dll_function_t)(launcher_t* launcher);

dll_function_t* dll_single_time_initialization;
dll_function_t* dll_initialize_after_hotload;
dll_function_t* dll_main_loop;
dll_function_t* dll_need_hotload;
dll_function_t* dll_shutdown;
void* game_dll;

#define CUTE_FILES_IMPLEMENTATION
#include <cute/cute_files.h>

cf_time_t prev_time, now_time;

void do_hotload(launcher_t* launcher);
void hotload_if_needed(launcher_t* launcher);

//--------------------------------------------------------------------------------------------------

void* read_file_to_memory(const char* file, int* size = 0)
{
	void* data = 0;
	FILE* fp = fopen(file, "rb");
	int sizeNum = 0;

	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		sizeNum = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		data = malloc(sizeNum);
		fread(data, sizeNum, 1, fp);
		fclose(fp);
	}

	if (size) *size = sizeNum;
	return data;
}

void copy_file_pointer(const char* from, const char* to)
{
	int size;
	void* memory = read_file_to_memory(from, &size);
	assert(size);
	FILE* fp = fopen(to, "wb");
	assert(fp);
	fwrite(memory, size, 1, fp);
	fflush(fp);
	fclose(fp);
	free(memory);
}

void do_hotload(launcher_t* launcher)
{
	if (!cf_file_exists("lock.tmp"))
	{
		printf("Hotloading...\n");
		SDL_UnloadObject(game_dll);
		copy_file_pointer("cute.dll", "temp_cute.dll");
		game_dll = SDL_LoadObject("temp_cute.dll");

		dll_single_time_initialization = (dll_function_t*)SDL_LoadFunction(game_dll, "single_time_initialization");
		dll_initialize_after_hotload = (dll_function_t*)SDL_LoadFunction(game_dll, "initialize_after_hotload");
		dll_main_loop = (dll_function_t*)SDL_LoadFunction(game_dll, "main_loop");
		dll_need_hotload = (dll_function_t*)SDL_LoadFunction(game_dll, "need_hotload");
		dll_shutdown = (dll_function_t*)SDL_LoadFunction(game_dll, "shutdown");

		cf_get_file_time("cute.dll", &now_time);
		printf("Done hotloading!\n");

		static int first_time = 0;
		if (!first_time) first_time = 1;
		else dll_initialize_after_hotload(launcher);
	}
}

void hotload_if_needed(launcher_t* launcher)
{
	if (!dll_need_hotload(launcher)) return;

	// Make sure all tasks are finished, and all pooled threads are sleeping
	// before performing a hotload.
	cute_threadpool_kick_and_wait(launcher->threadpool);

	printf("Compiling...\n");

	#if __PLATFORM__ == __WINDOWS__

		SetCurrentDirectoryA("..");
		system("build_game_dll.bat");
		SetCurrentDirectoryA("./bin");

	#else

		#error Platform not yet supported.

	#endif

	printf("Done compiling!\n");

	cf_get_file_time("cute.dll", &now_time);
	if (cf_compare_file_times(&now_time, &prev_time)) do_hotload(launcher);
}
