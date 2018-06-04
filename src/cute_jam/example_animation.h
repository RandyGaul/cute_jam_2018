void do_animation_demo(float dt);

//--------------------------------------------------------------------------------------------------

void do_animation_demo(float dt)
{
	static ani_t play_once;
	static ani_t loop_forwards;
	static ani_t loop_backwards;
	static ani_t loop_forwards_skip;
	static ani_t loop_backwards_skip;
	static int initialized;

	draw_text(env->courier_new, "Different loopings of one Clefairy animation.", 0, 200, 2);

	if (!initialized)
	{
		initialized = 1;
		play_once = get_ani("/data/sprites/clefairy/clefairy_spin.ani");
		loop_forwards = play_once;
		loop_backwards = play_once;
		loop_forwards_skip = play_once;
		loop_backwards_skip = play_once;
		play_once.cute_ani.looping = 0;
		loop_forwards.cute_ani.looping = 1;
		loop_backwards.cute_ani.looping = -1;
		loop_forwards_skip.cute_ani.looping = 2;
		loop_backwards_skip.cute_ani.looping = -2;
		loop_backwards_skip.cute_ani.current_frame = 1;
	}

	cute_ani_update(&play_once.cute_ani, dt);
	cute_ani_update(&loop_forwards.cute_ani, dt);
	cute_ani_update(&loop_backwards.cute_ani, dt);
	cute_ani_update(&loop_forwards_skip.cute_ani, dt);
	cute_ani_update(&loop_backwards_skip.cute_ani, dt);

	push_ani(&play_once, v2(0, 40), v2(2, 2));
	push_ani(&loop_forwards, v2(0, 0), v2(2, 2));
	push_ani(&loop_backwards, v2(0, -40), v2(2, 2));
	push_ani(&loop_forwards_skip, v2(0, 80), v2(2, 2));
	push_ani(&loop_backwards_skip, v2(0, -80), v2(2, 2));
}
