struct ani_t
{
	cute_ani_t cute_ani;
	sprite_t sprites[CUTE_ANI_MAX_FRAMES];
};

/**
 * Returns an animation based on the animation descriptor at `path`. `frame_count` will
 * be zero in the case of errors (like path does not exist, or parse errors).
 */
ani_t get_ani(const char* path);
void push_ani(ani_t* ani, v2 position);

void init_ani_system();
void shutdown_ani_system();
void free_ani(const char* path);

//--------------------------------------------------------------------------------------------------

ani_t get_ani(const char* path)
{
	uint64_t key = FNV1a(path, strlen(path));
	ani_t ani;
	ani.cute_ani.frame_count = 0;

	void* data;
	int sz;
	if (!load_file(path, &data, &sz))
	{
		return ani;
	}

	cute_ani_error_t status = cute_ani_load_from_mem(env->ani_map, &ani.cute_ani, data, sz, 0);
	if (status != CUTE_ANI_SUCCESS)
	{
		ani.cute_ani.frame_count = 0;
	}

	else
	{
		for (int i = 0; i < ani.cute_ani.frame_count; ++i)
		{
			const char* path = cute_ani_map_cstr(env->ani_map, ani.cute_ani.frames[i].image_id);
			ani.sprites[i] = get_sprite(path);
		}
	}

	return ani;
}

void push_ani(ani_t* ani, v2 position, v2 scale)
{
	sprite_t sprite = ani->sprites[ani->cute_ani.current_frame];
	sprite.x += position.x;
	sprite.y += position.y;
	sprite.sx *= scale.x;
	sprite.sy *= scale.y;
	push_sprite(sprite);
}

void init_ani_system()
{
	hashtable_init(&env->ani_table, sizeof(ani_t), 256, 0);
	env->ani_map = cute_ani_map_create(0);
}

void shutdown_ani_system()
{
	hashtable_term(&env->ani_table);
	cute_ani_map_destroy(env->ani_map);
}

void free_ani(const char* path)
{
	hashtable_remove(&env->ani_table, FNV1a(path, strlen(path)));
}
