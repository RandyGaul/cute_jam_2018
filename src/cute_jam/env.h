struct cute_font_t;
using font_t = cute_font_t;

struct env_t* env;

struct env_t
{
	// SDL and system globals.
	SDL_Window* window;
	SDL_GLContext ctx_sdl_gl;
	cute_mutex_t* assetsys_mutex;
	assetsys_t* assetsys;
	filewatch_t* filewatch;
	cute_threadpool_t* threadpool;
	float windowWidth, windowHeight;

	// Audio system.
	cs_context_t* ctx_cute_sound;
	struct audio_context_t* ctx_audio;
	struct audio_t* default_sound;

	// Graphics system.
	gl_context_t* ctx_gl;
	gl_shader_t sprite_shader;
	gl_renderable_t sprite_renderable;
	float projection[16];
	int screen_width_pixels;
	int screen_height_pixels;

	// Sprite registration system.
	int sprite_ids = 0;
	hashtable_t sprite_table;
	hashtable_t pixel_table;
	int checkerboard_loaded = 0;
	int spritebatch_ready = 0;
	spritebatch_t spritebatch;
	int sprite_verts_count = 0;
	int sprite_verts_capacity = 0;
	struct vertex_t* sprite_verts = 0;

	// Animation system.
	cute_ani_map_t* ani_map;
	hashtable_t ani_table;

	// Tile system.
	struct shape_t* shapes = 0;
	int shape_count = 0;

	// Font system.
	int font_verts_count = 0;
	int font_verts_capacity = 0;
	struct cute_font_vert_t* font_verts = 0;
	font_t* courier_new = 0;

	// Entity system.
	struct entity_t* entity_list = 0;

	// Temporary "just trying something out" stuff goes here.
	int which_demo = 0;
	int spritebatch_demo_scene = 0;
	int* tile_demo_shape_ids = 0;
	int tile_demo_tile_count = 0;
	struct tile_t* tile_demo_tiles = 0;
};

void add_entity_to_list(entity_t** list_ptr, entity_t* entity);
int load_file(const char* path, void** out, int* size);
void add_task(void (*func)(void* udata), void* param);
void threadpool_kick_and_wait();
void threadpool_kick();

void init_asset_system(const char* actual_path, const char* virtual_path);
void shutdown_asset_system();
void set_global_function_ptrs(launcher_t* launcher);

//--------------------------------------------------------------------------------------------------

void init_asset_system(const char* actual_path, const char* virtual_path)
{
	env->assetsys = assetsys_create(0);
	env->filewatch = filewatch_create(env->assetsys, 0);
	filewatch_mount(env->filewatch, actual_path, virtual_path);
}

void shutdown_asset_system()
{

	filewatch_dismount(env->filewatch);
	filewatch_free(env->filewatch);
	cute_lock(env->assetsys_mutex);
	assetsys_destroy(env->assetsys);
	env->assetsys = 0;
	cute_unlock(env->assetsys_mutex);
	cute_mutex_destroy(env->assetsys_mutex);
}

void set_global_function_ptrs(launcher_t* launcher)
{
	env = (env_t*)launcher->ctx_game;
	global_alloc = launcher->alloc_fn;
	global_free = launcher->free_fn;

	gladLoadGLES2Loader(SDL_GL_GetProcAddress);

	printf("Glad says OpenGL ES version : %d.%d\n", GLVersion.major, GLVersion.minor);
	printf("OpenGL says : ES %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}

#define AS_CHECK_ERROR(code) do { if (assetsys_check_code(code)) goto error; } while (0)
int assetsys_check_code(assetsys_error_t code)
{
	switch (code)
	{
		case ASSETSYS_ERROR_INVALID_PATH: printf("ASSETSYS_ERROR_INVALID_PATH\n"); return 1; 
		case ASSETSYS_ERROR_INVALID_MOUNT: printf("ASSETSYS_ERROR_INVALID_MOUNT\n"); return 1; 
		case ASSETSYS_ERROR_FAILED_TO_READ_ZIP: printf("ASSETSYS_ERROR_FAILED_TO_READ_ZIP\n"); return 1; 
		case ASSETSYS_ERROR_FAILED_TO_CLOSE_ZIP: printf("ASSETSYS_ERROR_FAILED_TO_CLOSE_ZIP\n"); return 1; 
		case ASSETSYS_ERROR_FAILED_TO_READ_FILE: printf("ASSETSYS_ERROR_FAILED_TO_READ_FILE\n"); return 1; 
		case ASSETSYS_ERROR_FILE_NOT_FOUND: printf("ASSETSYS_ERROR_FILE_NOT_FOUND\n"); return 1; 
		case ASSETSYS_ERROR_DIR_NOT_FOUND: printf("ASSETSYS_ERROR_DIR_NOT_FOUND\n"); return 1; 
		case ASSETSYS_ERROR_INVALID_PARAMETER: printf("ASSETSYS_ERROR_INVALID_PARAMETER\n"); return 1; 
		case ASSETSYS_ERROR_BUFFER_TOO_SMALL: printf("ASSETSYS_ERROR_BUFFER_TOO_SMALL\n"); return 1; 
	}

	return 0;
}

int load_file(const char* path, void** out, int* size)
{
	*out = 0;
	if (size) *size = 0;
	assetsys_file_t file;
	cute_lock(env->assetsys_mutex);
	AS_CHECK_ERROR(assetsys_file(env->assetsys, path, &file));
	int sz = assetsys_file_size(env->assetsys, file);

	void* mem = ALLOC((size_t)sz);
	while (1)
	{
		assetsys_error_t code = assetsys_file_load(env->assetsys, file, &sz, mem, sz);

		switch (code)
		{
		case ASSETSYS_ERROR_BUFFER_TOO_SMALL:
			FREE(mem);
			mem = ALLOC((size_t)sz);
			break;

		case ASSETSYS_SUCCESS:
			goto end;

		default:
			AS_CHECK_ERROR(code);
			// goes to error;
		}
	}

end:
	cute_unlock(env->assetsys_mutex);
	*out = mem;
	if (size) *size = sz;
	return 1;

error:
	cute_unlock(env->assetsys_mutex);
	return 0;
}

uint64_t FNV1a(const void* buf, int len)
{
	uint64_t h = (uint64_t)14695981039346656037;
	const char* str = (const char*)buf;

	while (len--)
	{
		char c = *str++;
		h = h ^ (uint64_t)c;
		h = h * (uint64_t)1099511628211;
	}

	return h;
}

#define CHECK_BUFFER_GROW(ctx, count, capacity, data, type, initial) \
	do { \
		if (ctx->count == ctx->capacity) \
		{ \
			int new_capacity = ctx->capacity ? ctx->capacity * 2 : initial; \
			void* new_data = ALLOC(sizeof(type) * new_capacity); \
			memcpy(new_data, ctx->data, sizeof(type) * ctx->count); \
			FREE(ctx->data); \
			ctx->data = (type*)new_data; \
			ctx->capacity = new_capacity; \
		} \
	} while (0)

#define BUFFER_GROW(ctx, count, capacity, data, type, new_cap) \
	do { \
		int new_capacity = new_cap; \
		void* new_data = ALLOC(sizeof(type) * new_capacity); \
		memcpy(new_data, ctx->data, sizeof(type) * ctx->count); \
		FREE(ctx->data); \
		ctx->data = (type*)new_data; \
		ctx->capacity = new_capacity; \
	} while (0)

void add_task(void (*func)(void* udata), void* param)
{
	cute_threadpool_add_task(env->threadpool, func, param);
}

void threadpool_kick_and_wait()
{
	cute_threadpool_kick_and_wait(env->threadpool);
}

void threadpool_kick()
{
	cute_threadpool_kick(env->threadpool);
}

#define DLIST_INIT(sentinel) \
	do { \
		(sentinel)->next = (sentinel); \
		(sentinel)->prev = (sentinel); \
	} while (0)

#define DLIST_INSERT(sentinel, element) \
	do { \
		(element)->prev = (sentinel); \
		(element)->next = (sentinel)->next; \
		(sentinel)->next->prev = (element); \
		(sentinel)->next = (element); \
	} while (0)

#define DLIST_REMOVE(element) \
	do { \
		(element)->prev->next = (element)->next; \
		(element)->next->prev = (element)->prev; \
	} while (0)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#include <crtdbg.h>
