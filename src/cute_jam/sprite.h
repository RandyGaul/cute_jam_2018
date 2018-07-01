#include <math.h>

struct sprite_t
{
	SPRITEBATCH_U64 image_id;
	int depth;
	float x, y;
	float image_w, image_h;
	float sx, sy;
	float c, s;
};

struct image_t
{
	cp_image_t img;
	SPRITEBATCH_U64 sprite_id;
};

#define push_sprite(sp) \
	spritebatch_push(&env->spritebatch, sp.image_id, sp.image_w, sp.image_h, sp.x, sp.y, sp.sx, sp.sy, sp.c, sp.s, (SPRITEBATCH_U64)sp.depth)

sprite_t get_sprite(const char* path);
void init_sprite_system();
void shutdown_sprite_system();
image_t load_image(const char* path);
void free_image(const char* path);

//--------------------------------------------------------------------------------------------------

const char* default_image_path = "/data/sprites/checkerboard.png";

image_t load_image(const char* path)
{
	uint64_t key = FNV1a(path, strlen(path));
	image_t* image_ptr = (image_t*)hashtable_find(&env->sprite_table, key);
	if (image_ptr) return *image_ptr;
	else
	{
		image_t image;
		void* data;
		int sz;

		// Load checkerboard default image in case the path is not found.
		if (!load_file(path, &data, &sz))
		{
			uint64_t default_key = FNV1a(default_image_path, strlen(default_image_path));

			if (!env->checkerboard_loaded)
			{
				env->checkerboard_loaded = 1;
				load_file(default_image_path, &data, &sz);
				image.img = cp_load_png_mem(data, sz);
				image.sprite_id = ++env->sprite_ids;
				hashtable_insert(&env->sprite_table, default_key, &image);
				hashtable_insert(&env->pixel_table, image.sprite_id, &image.img.pix);
				FREE(data);
				return image;
			}

			else
			{
				return *(image_t*)hashtable_find(&env->sprite_table, default_key);
			}
		}

		image.img = cp_load_png_mem(data, sz);
		image.sprite_id = ++env->sprite_ids;
		hashtable_insert(&env->sprite_table, key, &image);
		hashtable_insert(&env->pixel_table, image.sprite_id, &image.img.pix);
		FREE(data);
		return image;
	}
}

void free_image(const char* path)
{
	uint64_t key = FNV1a(path, strlen(path));
	image_t* image_ptr = (image_t*)hashtable_find(&env->sprite_table, key);
	if (image_ptr)
	{
		cp_free_png(&image_ptr->img);
		hashtable_remove(&env->pixel_table, image_ptr->sprite_id);
		hashtable_remove(&env->sprite_table, key);
	}
}

sprite_t get_sprite(const char* path)
{
	image_t image = load_image(path);

	sprite_t sprite;
	sprite.image_id = image.sprite_id;
	sprite.x = 0;
	sprite.y = 0;
	sprite.image_w = image.img.w;
	sprite.image_h = image.img.h;
	sprite.sx = image.img.w;
	sprite.sy = image.img.h;
	sprite.c = 1.0f;
	sprite.s = 0;

	return sprite;
}

void init_spritebatcher()
{
	// setup the sprite batcher
	spritebatch_config_t config;
	spritebatch_set_default_config(&config);
	config.pixel_stride = sizeof(cp_pixel_t);
	config.lonely_buffer_count_till_flush = 1;
	config.ticks_to_decay_texture = 1024;

	// Assign the four callbacks so the sprite batcher knows how to get
	// pixels whenever it needs them, and assign them GPU texture handles.
	config.batch_callback = batch_report;                       // report batches of sprites from `spritebatch_flush`
	config.get_pixels_callback = get_pixels;                    // used to retrieve image pixels from `spritebatch_flush` and `spritebatch_defrag`
	config.generate_texture_callback = generate_texture_handle; // used to generate a texture handle from `spritebatch_flush` and `spritebatch_defrag`
	config.delete_texture_callback = destroy_texture_handle;    // used to destroy a texture handle from `spritebatch_defrag`

	spritebatch_init(&env->spritebatch, &config);
	env->spritebatch_ready = 1;
}

void shutdown_spritebatcher()
{
	spritebatch_term(&env->spritebatch);
}

void init_sprite_system()
{
	init_spritebatcher();
	hashtable_init(&env->sprite_table, sizeof(image_t), 1024, 0);
	hashtable_init(&env->pixel_table, sizeof(void*), 1024, 0);
}

void shutdown_sprite_system()
{
	shutdown_spritebatcher();
	hashtable_term(&env->sprite_table);
	hashtable_term(&env->pixel_table);
	FREE(env->sprite_verts);
	env->spritebatch_ready = 0;
}
