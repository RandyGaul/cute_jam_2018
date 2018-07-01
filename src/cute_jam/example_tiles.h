void do_tile_demo();
void example_tiles_init();
void example_tiles_shutdown();

//--------------------------------------------------------------------------------------------------

void load_tile_demo_images()
{
	const char* path = "/data/tiles/game_tiles";
	char buf[256];

	for (int i = 0; i < 16; ++i)
	{
		sprintf(buf, "%s%d.png", path, i + 1);
		load_image(buf);
	}
}

void free_tile_demo_images()
{
	const char* path = "/data/tiles/game_tiles";
	char buf[256];

	for (int i = 0; i < 16; ++i)
	{
		sprintf(buf, "%s%d.png", path, i + 1);
		_CrtCheckMemory();
		free_image(buf);
	}
}

void example_tiles_init()
{
	// load the collision shape for each tile type
	int* shape_ids = load_tile_map_shape_ids("/data/tiles/collision.json", 0);
	env->tile_demo_shape_ids = shape_ids;

	// load up the tile image ids, positions, and transforms
	env->tile_demo_tiles = load_tile_map("/data/tiles/level0.json", &env->tile_demo_tile_count, shape_ids, "/data/tiles/game_tiles");

	load_tile_demo_images();
}

void example_tiles_shutdown()
{
	FREE(env->tile_demo_shape_ids);
	env->tile_demo_shape_ids = 0;
	FREE(env->tile_demo_tiles);
	env->tile_demo_tiles = 0;
	env->tile_demo_tile_count = 0;

	free_tile_demo_images();
}

void do_tile_demo()
{
	draw_text(env->courier_new, "Press space, a, d, or 1.", 0, 200, 2);

	// Draw the tiles.
	for (int i = 0; i < env->tile_demo_tile_count; ++i)
	{
		push_sprite(env->tile_demo_tiles[i].sprite);
	}
}

struct player_t : public entity_t
{
	player_t() { id = EID_PLAYER; }
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
	vec2 startPos;
};

player_t* create_player()
{
	player_t* player = NEW(player_t);
	player->startPos.x = player->quote_x;
	player->startPos.y = player->quote_y;
	player->quote_sprite = get_sprite("/data/cavestory/sprites/quote.png");
	player->quote_sprite.sx *= 2.0f;
	player->quote_sprite.sy *= 2.0f;
	player->quote_circle.r = player->quote_sprite.sy / 2.0f;
	
	env->playa = player;

	add_entity_to_list(&env->entity_list, player);
	return player;
}

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

void update_player(entity_t* entity, float dt)
{
	player_t* player = (player_t*)entity;

	// Debug draw tile collision shapes and Quote.
	if (key_down(KEY_1))
	{
		debug_draw_tile_shapes(env->tile_demo_tiles, env->tile_demo_tile_count);

		// Draw Quote circle shape.
		c2v p = player->quote_circle.p;
		float r = player->quote_circle.r;
		int kSegs = 40;
		float theta = 0;
		float inc = 3.14159265f * 2.0f / (float)kSegs;
		float px, py;
		c2SinCos( theta, &py, &px );
		px *= r; py *= r;
		px += p.x; py += p.y;
		for (int i = 0; i <= kSegs; ++i)
		{
			theta += inc;
			float x, y;
			c2SinCos( theta, &y, &x );
			x *= r; y *= r;
			x += p.x; y += p.y;
			gl_line(env->ctx_gl, x, y, 0, px, py, 0);
			px = x; py = y;
		}
	}

	// debug draw the mouse position
	v2 mousep(g_mouse.x - env->windowWidth / 2.0,
		-g_mouse.y + env->windowHeight / 2.0);
	//DrawDebugCircle(mousep, 10, 40);

	// debug draw the line from player to mouse
	//gl_line(env->ctx_gl, mousep.x, mousep.y, 0, ctx->quote_x,
	//ctx->quote_y, 0);

	// update the jetpack cooldown timer
	if (player->jetpackCD > 0)
	{
		player->jetpackCD -= dt; // subtract time from cooldown timer

		if (player->jetpackCD < 0) // if it goes below zero make it zero
			player->jetpackCD = 0;
	}

	// check if we should fire the jetpack
	if (player->jetpackCD == 0
		&& mouse_once(g_mouse.left_button)) // LMB pressed
	{
		v2 mouseDir(mousep.x - player->quote_x, mousep.y - player->quote_y);
		mouseDir = norm(mouseDir);

		player->quote_vel_x = mouseDir.x * 150.0;
		player->quote_vel_y = mouseDir.y * 150.0;

		player->jetpackCD += 1;
	}

	// Quote's physics integration.
	player->quote_vel_y += dt * -250.0f;
	player->quote_x += dt * player->quote_vel_x;
	player->quote_y += dt * player->quote_vel_y;
	player->quote_circle.p = c2V(player->quote_x, player->quote_y);
	player->quote_sprite.x = player->quote_x;
	player->quote_sprite.y = player->quote_y;

	// Quote's collision detection and collision resolution.
	for (int i = 0; i < env->tile_demo_tile_count; ++i)
	{
		tile_t tile = env->tile_demo_tiles[i];
		shape_t shape = get_transformed_tile_shape(tile);
		if (shape.type == C2_NONE) continue;

		c2Manifold m;
		c2Collide(&player->quote_circle, 0, C2_CIRCLE, &shape.u, 0, shape.type, &m);

		if (m.count)
		{
			//printf("%d\n",tile.tileID);
			switch (tile.tileID)
			{
			case 19:
				player->quote_x = player->startPos.x;
				player->quote_y = player->startPos.y;
				player->quote_vel_x = 0;
				break;
			default:
				break;
			}

			// Move quote out of colliding configuration.
			float depth = -m.depths[0];
			c2v n = m.n;
			player->quote_x += n.x * depth;
			player->quote_y += n.y * depth;

			// "slide along wall"
			// Remove all velocity in the direction of the wall's normal:
			// v -= n * dot(v, n)
			c2v v = c2V(player->quote_vel_x, player->quote_vel_y);
			v = c2Sub(v, c2Mulvs(n, c2Dot(v, n)));
			player->quote_vel_x = v.x;
			player->quote_vel_y = v.y;

			// tile interactions with player
			printf("%d\n", tile.tileID);
			switch (tile.tileID)
			{
			case 19: // spikes / death tile
				player->quote_x = player->startPos.x;
				player->quote_y = player->startPos.y;
				player->quote_vel_x = 0;
				player->quote_vel_y = 0;
				break;
			case 44: // bouncy
				player->quote_vel_y += 450;
				break;
			default:
				break;
			}
		}
	}
}

void draw_player(entity_t* entity)
{
	player_t* player = (player_t*)entity;

	// Draw Quote.
	if (player->dir == 1) player->quote_sprite.sx *= c2Sign(player->quote_sprite.sx) < 0 ? -1.0f : 1.0f;
	if (player->dir == 2) player->quote_sprite.sx *= c2Sign(player->quote_sprite.sx) < 0 ? 1.0f : -1.0f;
	push_sprite(player->quote_sprite);
}
