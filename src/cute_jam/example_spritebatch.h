// example file/asset i/o system
const char* image_paths[] = {
	"/data/cavestory/sprites/basu.png",
	"/data/cavestory/sprites/bat.png",
	"/data/cavestory/sprites/behemoth.png",
	"/data/cavestory/sprites/crow.png",
	"/data/cavestory/sprites/dragon_zombie.png",
	"/data/cavestory/sprites/fire_whirl.png",
	"/data/cavestory/sprites/giant_pignon.png",
	"/data/cavestory/sprites/night_spirit.png",
	"/data/cavestory/sprites/orangebell.png",
	"/data/cavestory/sprites/petit.png",
	"/data/cavestory/sprites/polish.png",
	"/data/cavestory/sprites/power_critter.png",
};

int images_count = sizeof(image_paths) / sizeof(*image_paths);
image_t images[sizeof(image_paths) / sizeof(*image_paths)];

sprite_t make_sprite(const char* path, float x, float y, float scale, float angle_radians, int depth)
{
	sprite_t s = get_sprite(path);
	s.depth = depth;
	s.x = x;
	s.y = y;
	s.sx = s.sx * 2.0f * scale;
	s.sy = s.sy * 2.0f * scale;
	s.c = cosf(angle_radians);
	s.s = sinf(angle_radians);
	return s;
}

void example_spritebatch_load_images()
{
	for (int i = 0; i < images_count; ++i)
	{
		image_t image = load_image(image_paths[i]);
		images[i] = image;
	}
}

void example_spritebatch_unload_images()
{
	for (int i = 0; i < images_count; ++i)
	{
		free_image(image_paths[i]);
	}
}

void example_spritebatch_scene0()
{
	sprite_t basu = make_sprite(image_paths[0], 0, 0, 1.0f, 0, 0);
	sprite_t bat = make_sprite(image_paths[1], 30, 30, 1.0f, 0, 0);
	sprite_t behemoth = make_sprite(image_paths[2], 80, 30, 1.0f, 3.14159265f / 4.0f, 0);
	sprite_t crow = make_sprite(image_paths[3], 70, -50, 1.0f, -3.14159265f / 4.0f, 0);

	push_sprite(basu);
	push_sprite(bat);
	push_sprite(behemoth);
	push_sprite(crow);
}

void example_spritebatch_scene1()
{
	sprite_t basu = make_sprite(image_paths[0], 0, 0, 1.0f, 0, 0);
	sprite_t bat = make_sprite(image_paths[1], 30, 30, 1.0f, 0, 0);
	push_sprite(basu);
	push_sprite(bat);
}

void example_spritebatch_scene2()
{
	sprite_t basu = make_sprite(image_paths[0], 0, 0, 1.0f, 0, 0);
	sprite_t bat = make_sprite(image_paths[1], 30, 30, 1.0f, 0, 0);
	sprite_t behemoth = make_sprite(image_paths[2], 80, 30, 1.0f, 3.14159265f / 4.0f, 0);
	sprite_t crow = make_sprite(image_paths[3], 70, -50, 1.0f, -3.14159265f / 4.0f, 0);

	static int which = 0;

	switch (which)
	{
	case 0: push_sprite(basu);     break;
	case 1: push_sprite(bat);      break;
	case 2: push_sprite(behemoth); break;
	case 3: push_sprite(crow);     break;
	}

	which = (which + 1) % 4;
}

void example_spritebatch_scene3()
{
	sprite_t dragon_zombie = make_sprite(image_paths[4], -250, -200, 1, 0, 0);
	sprite_t fire_whirl = make_sprite(image_paths[5], -150, -100, 1, 0, 0);
	sprite_t giant_pignon = make_sprite(image_paths[6], -200, 0, 1, 0, 0);
	sprite_t night_spirit = make_sprite(image_paths[7], -225, 100, 1, 0, 0);
	sprite_t orangebell = make_sprite(image_paths[8], -200, 200, 1, 0, 0);
	sprite_t petit = make_sprite(image_paths[9], -100, 200, 1, 0, 0);
	sprite_t power_critter = make_sprite(image_paths[11], -25, 75, 1, 0, 0);

	push_sprite(dragon_zombie);
	push_sprite(fire_whirl);
	push_sprite(giant_pignon);
	push_sprite(night_spirit);
	push_sprite(orangebell);
	push_sprite(petit);
	push_sprite(power_critter);

	sprite_t polish = make_sprite(image_paths[10], 50, 180, 1, 0, 0);
	sprite_t translated = polish;
	for (int i = 0; i < 4; ++i)
	{
		translated.x = polish.x + polish.sx * i;

		for (int j = 0; j < 6; ++j)
		{
			translated.y = polish.y - polish.sy * j;
			push_sprite(translated);
		}
	}
}

void do_spritebatch_example()
{
	draw_text(env->courier_new, "Press space to view four different scenes.", 0, 200, 2);

	void (*scenes[])() = {
		example_spritebatch_scene0,
		example_spritebatch_scene1,
		example_spritebatch_scene2,
		example_spritebatch_scene3,
	};

	if (key_once(KEY_SPACE))
	{
		env->spritebatch_demo_scene = (env->spritebatch_demo_scene + 1) % (sizeof(scenes) / sizeof(*scenes));
	}

	// push some sprites to cute_spritebatch
	scenes[env->spritebatch_demo_scene]();
}
