struct sound_parameters_t
{
	float volume = 1.0f;
	float panning = 0.5f;
	int looping = 0;
	float delay_before_play = 0;
};

sound_parameters_t sound_parameters_default;

cs_playing_sound_t* play_sound(const char* path, sound_parameters_t params = sound_parameters_default);
int play_music(const char* path, float crossfade_time = 2.0f, sound_parameters_t params = sound_parameters_default);
void update_audio(float dt);

void init_audio();
void shutdown_audio();

struct audio_t* prefetch_audio(const char* path);

//--------------------------------------------------------------------------------------------------

const char* default_audio_path = "/data/sounds/oof.wav";

enum streaming_state_t
{
	STREAMING_LOADED,
	STREAMING_LOADING,
	STREAMING_UNLOADED
};

struct audio_t
{
	const char* path;
	uint64_t key;
	int is_ogg;
	streaming_state_t streaming_state;
	cs_loaded_sound_t sound;
};

struct audio_context_t
{
	float crossfade_time = 1.0f;
	float crossfade_timer = 0;
	int changing_music = 0;
	audio_t* music_current = 0;
	audio_t* music_next = 0;
	cs_playing_sound_t* music_current_instance = 0;
	cs_playing_sound_t* music_next_instance = 0;
	sound_parameters_t music_next_params;

	int load_tasks_remaining = 0;
	cute_mutex_t* audio_table_mutex;
	hashtable_t audio_table;
};

cs_playing_sound_t* play_sound_internal(audio_t* audio, sound_parameters_t params)
{
	int loaded = audio->streaming_state == STREAMING_LOADED;
	cs_play_sound_def_t def = loaded ? cs_make_def(&audio->sound) : cs_make_def(&env->default_sound->sound);
	def.volume_left = params.volume;
	def.volume_right = params.volume;
	def.pan = params.panning;
	def.looped = params.looping;
	def.delay = params.delay_before_play;
	cs_playing_sound_t* playing_sound_instance = cs_play_sound(env->ctx_cute_sound, def);
	return loaded ? playing_sound_instance : 0;
}

cs_playing_sound_t* play_sound(const char* path, sound_parameters_t params)
{
	audio_t* audio = prefetch_audio(path);
	return play_sound_internal(audio, params);
}

int play_music(const char* path, float crossfade_time, sound_parameters_t params)
{
	audio_context_t* ctx = env->ctx_audio;

	// Ignore if already in the process of changing the music.
	if (ctx->changing_music) return 0;

	ctx->changing_music = 1;

	if (path)
	{
		// Start changing the music to the new song.
		audio_t* audio = prefetch_audio(path);
		ctx->music_next_instance = play_sound_internal(audio, params);
		ctx->music_next = audio;
		ctx->crossfade_time = crossfade_time;
		ctx->music_next_params = params;
	}

	return 1;
}

int is_ready_to_crossfade(audio_t* audio)
{
	if (audio)
	{
		return audio->streaming_state == STREAMING_LOADED;
	}

	else
	{
		return 1;
	}
}

float smoothstep(float x)
{
	return x * x * (3.0f - 2.0f * x);
}

void update_audio(float dt)
{
	audio_context_t* ctx = env->ctx_audio;

	// Crossfading logic for music.
	if (ctx->changing_music)
	{
		if (is_ready_to_crossfade(ctx->music_current) && is_ready_to_crossfade(ctx->music_next))
		{
			ctx->crossfade_timer += dt;
			float t = ctx->crossfade_timer / ctx->crossfade_time;

			if (t >= 1.0f)
			{
				// Free current audio.
				if (ctx->music_current && ctx->music_current != ctx->music_next)
				{
					cs_lock(env->ctx_cute_sound);
					cute_lock(ctx->audio_table_mutex);

					cs_stop_sound(ctx->music_current_instance);
					cs_free_sound(&ctx->music_current->sound);
					hashtable_remove(&ctx->audio_table, ctx->music_current->key);
					FREE(ctx->music_current);

					cute_unlock(ctx->audio_table_mutex);
					cs_unlock(env->ctx_cute_sound);
				}

				// Stop current, and swap with next.
				ctx->music_current_instance = ctx->music_next_instance;
				ctx->music_next_instance = 0;

				ctx->music_current = ctx->music_next;
				ctx->music_next = 0;

				ctx->crossfade_time = 0;
				ctx->crossfade_timer = 0;
				ctx->changing_music = 0;
			}

			else
			{
				cs_playing_sound_t* current = ctx->music_current_instance;
				cs_playing_sound_t* next = ctx->music_next_instance;
				if (current) current->volume0 = current->volume1 = smoothstep(1.0f - t);
				if (next) next->volume0 = next->volume1 = smoothstep(t);
				else
				{
					if (ctx->music_next)
					{
						next = play_sound_internal(ctx->music_next, ctx->music_next_params);
						ctx->music_next_instance = next;
						ctx->crossfade_timer = 0;
						t = ctx->crossfade_timer / ctx->crossfade_time;
						next->volume0 = next->volume1 = smoothstep(t);
					}
				}
			}
		}
	}
}

void load_audio_task(void* param)
{
	audio_t* audio = (audio_t*)param;
	audio->streaming_state = STREAMING_LOADING;
	void* data;
	int sz;
	int success = load_file(audio->path, &data, &sz);

	if (success)
	{
		if (audio->is_ogg)
		{
			cs_read_mem_ogg(data, sz, &audio->sound);
		}

		else
		{
			cs_read_mem_wav(data, sz, &audio->sound);
		}

		audio->streaming_state = STREAMING_LOADED;
		printf("Loaded: %s\n", audio->path);
		FREE(data);
	}

	env->ctx_audio->load_tasks_remaining -= 1;
}

audio_t* prefetch_audio(const char* path)
{
	audio_context_t* ctx = env->ctx_audio;
	uint64_t h = FNV1a(path, strlen(path));

	cute_lock(ctx->audio_table_mutex);
	audio_t** ptr = (audio_t**)hashtable_find(&ctx->audio_table, h);
	audio_t* audio = ptr ? *ptr : 0;
	cute_unlock(ctx->audio_table_mutex);

	if (!audio)
	{
		audio = NEW(audio_t);
		audio->path = path;
		audio->key = h;
		audio->streaming_state = STREAMING_UNLOADED;

		char ext[CUTE_PATH_MAX_EXT];
		path_pop_ext(path, 0, ext);
		if (!strcmp(ext, "ogg")) audio->is_ogg = 1;
		else audio->is_ogg = 0;

		cute_lock(ctx->audio_table_mutex);
		hashtable_insert(&ctx->audio_table, h, &audio);
		cute_unlock(ctx->audio_table_mutex);

		ctx->load_tasks_remaining += 1;
		add_task(load_audio_task, audio);
		threadpool_kick();

		return audio;
	}

	return audio;
}

void init_audio()
{
	env->ctx_audio = NEW(audio_context_t);
	audio_context_t* ctx = env->ctx_audio;
	hashtable_init(&ctx->audio_table, sizeof(audio_t*), 1536, 0);
	ctx->audio_table_mutex = cute_mutex_create();
	env->default_sound = prefetch_audio(default_audio_path);
}

void shutdown_audio()
{
	audio_context_t* ctx = env->ctx_audio;

	// Stop all playing sounds, and wait for them to be removed.
	cs_stop_all_sounds(env->ctx_cute_sound);
	while (env->ctx_cute_sound->playing) CUTE_THREAD_YIELD();

	// Free up all audio samples.
	audio_t** entries = (audio_t**)hashtable_items(&ctx->audio_table);
	int count = hashtable_count(&ctx->audio_table);
	for (int i = 0; i < count; ++i)
	{
		cs_free_sound(&entries[i]->sound);
	}

	// Destroy the audio table only once it is known no more load tasks
	// are running or waiting to run.
	while (ctx->load_tasks_remaining) CUTE_THREAD_YIELD();
	cute_lock(ctx->audio_table_mutex);
	hashtable_term(&ctx->audio_table);
	cute_unlock(ctx->audio_table_mutex);
	cute_mutex_destroy(ctx->audio_table_mutex);

	FREE(ctx);
	env->ctx_audio = 0;
}
