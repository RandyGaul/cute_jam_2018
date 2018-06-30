void do_tile_demo(float dt);
void example_tiles_init();
void example_tiles_shutdown();

//--------------------------------------------------------------------------------------------------

void load_tile_demo_images()
{
	const char* path = "/data/cavestory/tiles/cavestory_tiles";
	char buf[256];

	for (int i = 0; i < 2736; ++i)
	{
		sprintf(buf, "%s%d.png", path, i + 1);
		load_image(buf);
	}
}

void free_tile_demo_images()
{
	const char* path = "/data/cavestory/tiles/cavestory_tiles";
	char buf[256];

	for (int i = 0; i < 2736; ++i)
	{
		sprintf(buf, "%s%d.png", path, i + 1);
		free_image(buf);
	}
}

void example_tiles_init()
{
	// load the collision shape for each tile type
	int* shape_ids = load_tile_map_shape_ids("/data/cavestory/tiles/collision.json", 0);
	env->tile_demo_shape_ids = shape_ids;

	// load up the tile image ids, positions, and transforms
	env->tile_demo_tiles = load_tile_map("/data/cavestory/tiles/cavestory_tiles.json", &env->tile_demo_tile_count, shape_ids, "/data/cavestory/tiles/cavestory_tiles");

	load_tile_demo_images();
}

void example_tiles_shutdown()
{
	FREE(env->tile_demo_shape_ids);
	env->tile_demo_shape_ids = 0;
	FREE(env->tile_demo_tiles);
	env->tile_demo_tiles = 0;
	env->tile_demo_tile_count = 0;
	FREE(env->example_tile_data);

	free_tile_demo_images();
}

struct example_tiles_t
{
	c2Circle quote_circle;
	float quote_x = 0;
	float quote_y = 0;
	float quote_vel_x = 0;
	float quote_vel_y = 0;
	int debug_draw_shapes = 1;
	int dir = 0;
	sprite_t quote_sprite;
	float jetpackCD = 0; // 0 is ready to use
	float jetpackPower = 150.0;
};

void DrawDebugCircle(c2v p, float r, int kSegs, float _r = 1, float _g = 1, float _b = 1)
{
	gl_line_color(env->ctx_gl, _r, _g, _b);

	float theta = 0;
	float inc = 3.14159265f * 2.0f / (float)kSegs;
	float px, py;
	c2SinCos(theta, &py, &px);
	px *= r; py *= r;
	px += p.x; py += p.y;
	for (int i = 0; i <= kSegs; ++i)
	{
		theta += inc;
		float x, y;
		c2SinCos(theta, &y, &x);
		x *= r; y *= r;
		x += p.x; y += p.y;
		gl_line(env->ctx_gl, x, y, 0, px, py, 0);
		px = x; py = y;
	}
}

void DrawDebugCircle(v2 p, float r, int kSegs, float _r = 1, float _g = 1, float _b = 1)
{
	DrawDebugCircle(c2V(p.x, p.y), r, kSegs, _r, _g, _b);
}

void ManifoldDebugDrawing(c2Manifold m)
{
	example_tiles_t* ctx = env->example_tile_data;
	for (int i = 0; i < m.count; ++i)
	{
		DrawDebugCircle(m.contact_points[i], 5, 20);
		gl_line(env->ctx_gl, m.contact_points[i].x, m.contact_points[i].y, 0,
			m.contact_points[i].x + m.n.x * 10, m.contact_points[i].y + m.n.y * 10, 0);
	}
}

void do_tile_demo(float dt)
{
	draw_text(env->courier_new, "Press space, a, d, or 1.", 0, 200, 2);

	example_tiles_t* ctx = env->example_tile_data;

	// First time initialization.
	if (!ctx)
	{
		env->example_tile_data = NEW(example_tiles_t);
		ctx = env->example_tile_data;

		ctx->quote_sprite = get_sprite("/data/cavestory/sprites/quote.png");
		ctx->quote_sprite.sx *= 2.0f;
		ctx->quote_sprite.sy *= 2.0f;
		ctx->quote_circle.r = ctx->quote_sprite.sy / 2.0f;
	}

	// Draw the tiles.
	for (int i = 0; i < env->tile_demo_tile_count; ++i)
	{
		push_sprite(env->tile_demo_tiles[i].sprite);
	}

	// Debug draw tile collision shapes and Quote.
	if (key_down(KEY_1))
	{
		debug_draw_tile_shapes(env->tile_demo_tiles, env->tile_demo_tile_count);

		// Draw Quote circle shape.
		DrawDebugCircle(ctx->quote_circle.p, ctx->quote_circle.r, 40);
	}

	// debug draw the mouse position
	v2 mousep(g_mouse.x - env->windowWidth / 2.0,
		-g_mouse.y + env->windowHeight / 2.0);
	DrawDebugCircle(mousep, 10, 40);

	// debug draw the line from player to mouse
	//gl_line(env->ctx_gl, mousep.x, mousep.y, 0, ctx->quote_x,
	//ctx->quote_y, 0);

	// update the jetpack cooldown timer
	if (ctx->jetpackCD > 0)
	{
		ctx->jetpackCD -= dt; // subtract time from cooldown timer

		if (ctx->jetpackCD < 0) // if it goes below zero make it zero
			ctx->jetpackCD = 0;
	}

	// check if we should fire the jetpack
	if (ctx->jetpackCD == 0
		&& mouse_once(g_mouse.left_button)) // LMB pressed
	{
		v2 mouseDir(mousep.x - ctx->quote_x, mousep.y - ctx->quote_y);
		mouseDir = norm(mouseDir);

		ctx->quote_vel_x = mouseDir.x * 150.0;
		ctx->quote_vel_y = mouseDir.y * 150.0;
		
		ctx->jetpackCD += 1;
	}

	/*if (key_once(KEY_SPACE))
	{
		ctx->quote_vel_y = 250.0f;
	}*/

	/*if (key_down(KEY_A))
	{
		ctx->dir = 1;
	}

	else if (key_down(KEY_D))
	{
		ctx->dir = 2;
	}

	else
	{
		ctx->dir = 0;
	}*/

	/*switch (ctx->dir)
	{
	case 0: ctx->quote_vel_x = 0; break;
	case 1: ctx->quote_vel_x = -100.0f; break;
	case 2: ctx->quote_vel_x = 100.0f; break;
	}*/

	// Quote's physics integration.
	ctx->quote_vel_y += dt * -250.0f;
	ctx->quote_x += dt * ctx->quote_vel_x;
	ctx->quote_y += dt * ctx->quote_vel_y;
	ctx->quote_circle.p = c2V(ctx->quote_x, ctx->quote_y);
	ctx->quote_sprite.x = ctx->quote_x;
	ctx->quote_sprite.y = ctx->quote_y;

	// Quote's collision detection and collision resolution.
	for (int i = 0; i < env->tile_demo_tile_count; ++i)
	{
		tile_t tile = env->tile_demo_tiles[i];
		shape_t shape = get_transformed_tile_shape(tile);
		if (shape.type == C2_NONE) continue;

		c2Manifold m;
		c2Collide(&ctx->quote_circle, 0, C2_CIRCLE, &shape.u, 0, shape.type, &m);

		if (m.count) // if anything collided
		{
			// check if was below us


			ManifoldDebugDrawing(m);


			// Move quote out of colliding configuration.
			float depth = -m.depths[0];
			c2v n = m.n;
			ctx->quote_x += n.x * depth;
			ctx->quote_y += n.y * depth;

			v2 x;

			// "slide along wall"
			// Remove all velocity in the direction of the wall's normal:
			// v -= n * dot(v, n)
			c2v v = c2V(ctx->quote_vel_x, ctx->quote_vel_y);
			v = c2Sub(v, c2Mulvs(n, c2Dot(v, n)));
			ctx->quote_vel_x = v.x;
			ctx->quote_vel_y = v.y;
		}
	}

	// Draw Quote.
	if (ctx->dir == 1) ctx->quote_sprite.sx *= c2Sign(ctx->quote_sprite.sx) < 0 ? -1.0f : 1.0f;
	if (ctx->dir == 2) ctx->quote_sprite.sx *= c2Sign(ctx->quote_sprite.sx) < 0 ? 1.0f : -1.0f;
	push_sprite(ctx->quote_sprite);
}
