#define CUTE_TILED_ALLOC(size, ctx) ALLOC(size)
#define CUTE_TILED_FREE(mem, ctx) FREE(mem)
#define CUTE_TILED_IMPLEMENTATION
#include <cute/cute_tiled.h>

#define CUTE_TIME_IMPLEMENTATION
#include <cute/cute_time.h>

#define CUTE_PNG_ALLOC(size) ALLOC(size)
#define CUTE_PNG_FREE(ptr) FREE(ptr)
#define CUTE_PNG_CALLOC(count, size) CALLOC(count, size)
#define CUTE_PNG_IMPLEMENTATION
#include <cute/cute_png.h>

#define ASSETSYS_MALLOC(ctx, size) ALLOC(size)
#define ASSETSYS_FREE(ctx, ptr) FREE(ptr)
#define ASSETSYS_IMPLEMENTATION
#include <mattiasgustavsson/assetsys.h>

#define CUTE_FILEWATCH_MALLOC(size, ctx) ALLOC(size)
#define CUTE_FILEWATCH_FREE(ptr, ctx) FREE(ptr)
#define CUTE_FILEWATCH_IMPLEMENTATION
#include <cute/cute_filewatch.h>

#define CUTE_C2_IMPLEMENTATION
#include <cute/cute_c2.h>

#define SPRITEBATCH_MALLOC(size, ctx) ALLOC(size)
#define SPRITEBATCH_FREE(ptr, ctx) FREE(ptr)
#define SPRITEBATCH_IMPLEMENTATION
#include <cute/cute_spritebatch.h>

#define STRPOOL_MALLOC(ctx, size) ALLOC(size)
#define STRPOOL_FREE(ctx, ptr) FREE(ptr)
#define STRPOOL_IMPLEMENTATION
#include <mattiasgustavsson/strpool.h>

#define CUTE_SOUND_ALLOC(size) ALLOC(size)
#define CUTE_SOUND_FREE(ptr) FREE(ptr)
#define CUTE_SOUND_IMPLEMENTATION
#include <cute/cute_sound.h>

#define CUTE_SYNC_IMPLEMENTATION
#define CUTE_SYNC_SDL
#include <cute/cute_sync.h>

#define CUTE_PATH_IMPLEMENTATION
#include <cute/cute_path.h>

#define CUTE_GL_IMPLEMENTATION
#define CUTE_GL_MALLOC(size) ALLOC(size)
#define CUTE_GL_FREE(ptr) FREE(ptr)
#include <cute/cute_gl.h>

#define CUTE_ANI_IMPLEMENTATION
#define CUTE_ANI_ALLOC(size, ctx) ALLOC(size)
#define CUTE_ANI_FREE(ptr, ctx) FREE(ptr)
#include <cute/cute_ani.h>

#define CUTE_FONT_IMPLEMENTATION
#define CUTE_FONT_ALLOC(size, ctx) ALLOC(size)
#define CUTE_FONT_FREE(ptr, ctx) FREE(ptr)
#include <cute/cute_font.h>

#include <cute/cute_math2d.h>
