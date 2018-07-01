void do_audio_example()
{
	draw_text(env->courier_new, "Press 1, 2 or 3 to change music.\nPress space to play sound effect.", 0, 200, 2);

	if (key_once(KEY_SPACE))
	{
		play_sound("/data/sounds/jump.wav");
	}

	if (key_once(KEY_1))
	{
		play_music("/data/music/overworld_cute.wav");
	}

	if (key_once(KEY_2))
	{
		play_music("/data/music/Cave Story.ogg");
	}

	if (key_once(KEY_3))
	{
		play_music("/data/music/Cemetery.ogg");
	}
}
