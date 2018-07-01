// Include low level system headers.
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#define STB_VORBIS_HEADER_ONLY
#include <stb/stb_vorbis.c>

#define CUTE_SYNC_IMPLEMENTATION
#define CUTE_SYNC_SDL
#include <cute/cute_sync.h>

#include <launcher.h>
#include <cute_jam/alloc.h>
#include <cute_jam/cute_headers.h>

// Include all game headers here (carefully, in the right order).
#include <cute_jam/env.h>
#include <cute_jam/input.h>
#include <cute_jam/audio.h>
#include <cute_jam/graphics.h>
#include <cute_jam/sprite.h>
#include <cute_jam/animation.h>
#include <cute_jam/tiles.h>
#include <cute_jam/font.h>
#include <cute_jam/entity.h>

// Examples to demonstrate doing various things.
#include <cute_jam/example_audio.h>
#include <cute_jam/example_spritebatch.h>
#include <cute_jam/example_tiles.h>
#include <cute_jam/example_animation.h>

#include <cute_jam/coin.h>
#include <cute_jam/dog.h>
#include <cute_jam/mask_shader.h>

#include <cute_jam/load_objects_from_map_cpp.h>

// This must come last (or at least after all entity types, so the global vtable can be constructed).
#include <cute_jam/entity_cpp.h>

EXPORT int single_time_initialization(launcher_t* launcher)
{
	// Setup initial global variables and state.
	global_alloc = launcher->alloc_fn;
	global_free = launcher->free_fn;
	launcher->ctx_game = NEW(env_t);
	env = (env_t*)launcher->ctx_game;
	env->ctx_sdl_gl = launcher->ctx_sdl_gl;
	env->ctx_cute_sound = launcher->ctx_cute_sound;
	env->assetsys_mutex = cute_mutex_create();
	env->threadpool = launcher->threadpool;
	env->window = launcher->window;
	set_global_function_ptrs(launcher);
	SDL_GL_GetDrawableSize(env->window, &env->screen_width_pixels, &env->screen_height_pixels);

	// Initialize subsystems.
	init_asset_system("../data", "/data");
	init_audio();
	init_graphics(env->screen_width_pixels, env->screen_height_pixels);
	init_sprite_system();
	init_ani_system();
	init_tile_system();
	init_font_system();

	// Extra temporary stuff.
	example_spritebatch_load_images();
	example_tiles_init();

	// Setup the player.
	env->player = create_player();
	create_dog();

	init_coins();

	setup_mask_shader();



	srand(time(0));

	return 0;
}

EXPORT int initialize_after_hotload(launcher_t* launcher)
{
	set_global_function_ptrs(launcher);
	if (env->spritebatch_ready)
	{
		spritebatch_reset_function_ptrs(&env->spritebatch, batch_report, get_pixels, generate_texture_handle, destroy_texture_handle);
	}
	return 0;
}

EXPORT int main_loop(launcher_t* launcher)
{
	// Calc dt, and get SDL events (like user input).
	float dt = ct_time();
	pump_SDL_messages();

	env->game_time += dt;

	// get the window width and height
	int w, h;
	SDL_GetWindowSize(env->window, &w, &h);
	env->windowWidth = w;
	env->windowHeight = h;

	// Calc and display fps
	static float dt_accum;
	static uint64_t tick_count = 0;
	dt_accum = (dt - dt_accum) * (1.0f / 10.0f) + dt_accum;
	if (dt_accum < 0) dt_accum = 0;
	if (tick_count++ % 10 == 0)
	{
		float fps = 1.0f / dt_accum;
		if (fps > 10000.0f) printf("fps : Really fast\n");
		else printf("fps : %f\n", fps);
	}

	update_audio(dt);

	// Run the examples. These are good demonstrations of important engine features.
	if (key_once(KEY_RIGHT))
	{
		env->which_demo = ++env->which_demo % 4;
	}

	switch (env->which_demo)
	{
	case 0: do_audio_example(); break;
	case 1: do_spritebatch_example(); break;
	case 2: do_animation_demo(dt); break;
	case 3: do_tile_demo(); break;
	}

	draw_text(env->courier_new, "Press right arrow key to change demo.", 0, 220, 2);

	update_mask_shader();

	// Display the number of coins collected
	char coin_text[16];
	sprintf(coin_text, "Coins: %d", env->coin_count);
	draw_text(env->courier_new, coin_text, -280, 230, 2);

	update_entities(env->entity_list, dt);
	draw_entities(env->entity_list);

	// Update spritebatch system.
	spritebatch_defrag(&env->spritebatch);
	spritebatch_tick(&env->spritebatch);
	spritebatch_flush(&env->spritebatch);

	// Render to the screen.
	gl_flush(env->ctx_gl, swap_buffers, 0, 640, 480);
	CUTE_GL_PRINT_GL_ERRORS();

	// Reset vertex buffers.
	env->font_verts_count = 0;
	env->sprite_verts_count = 0;

	return application_running;
}

EXPORT int need_hotload(launcher_t* launcher)
{
	int ctrl = key_down(KEY_LCTRL) | key_down(KEY_RCTRL);
	if (ctrl && key_down(KEY_R)) return 1;
	else return 0;
}

EXPORT int shutdown(launcher_t* launcher)
{
	// Shutdown extra temporary stuff.
	example_spritebatch_unload_images();
	example_tiles_shutdown();

	// Shutdown subsystems.
	shutdown_audio();
	shutdown_asset_system();
	shutdown_graphics();
	shutdown_sprite_system();
	shutdown_ani_system();
	shutdown_tile_system();
	shutdown_font_system();

	// Free the global game environment variable.
	FREE(env);
	launcher->ctx_game = 0;
	return 0;
}

#undef STB_VORBIS_HEADER_ONLY
#include <stb/stb_vorbis.c>
