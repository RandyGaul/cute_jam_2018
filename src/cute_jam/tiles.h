struct shape_t
{
	union
	{
		c2Circle circle;
		c2AABB aabb;
		c2Capsule capsule;
		c2Poly poly;
	} u;

	C2_TYPE type;
};

struct tile_t
{
	int x, y;
	sprite_t sprite;
	int shape_id;
};

int* load_tile_map_shape_ids(const char* path, int* tile_count_out);
tile_t* load_tile_map(const char* path, int* tile_count_out, int* shape_ids, const char* image_path_base);
shape_t get_transformed_tile_shape(tile_t tile);
void debug_draw_tile_shapes(tile_t* tiles, int tile_count);

//--------------------------------------------------------------------------------------------------

void init_tile_shapes()
{
	c2AABB bb;
	bb.min = c2V(-0.5f, -0.5f);
	bb.max = c2V(0.5f, 0.5f);

	c2Poly tri_smaller;
	tri_smaller.verts[0] = c2V(-0.5f, -0.5f);
	tri_smaller.verts[1] = c2V(0.5f, -0.5f);
	tri_smaller.verts[2] = c2V(0.5f, 0);
	tri_smaller.count = 3;
	c2Norms(tri_smaller.verts, tri_smaller.norms, 3);

	c2Poly tri_larger;
	tri_larger.verts[0] = c2V(-0.5f, -0.5f);
	tri_larger.verts[1] = c2V(0.5f, -0.5f);
	tri_larger.verts[2] = c2V(0.5f, 0.5f);
	tri_larger.verts[3] = c2V(-0.5f, 0);
	tri_larger.count = 4;
	c2Norms(tri_larger.verts, tri_larger.norms, 3);

	shape_t shape0;
	shape0.u.poly = tri_larger;
	shape0.type = C2_POLY;

	shape_t shape1;
	shape1.u.poly = tri_smaller;
	shape1.type = C2_POLY;

	shape_t shape2;
	shape2.u.aabb = bb;
	shape2.type = C2_AABB;

	env->shapes = (shape_t*)ALLOC(sizeof(shape_t) * 3);
	env->shapes[0] = shape0;
	env->shapes[1] = shape1;
	env->shapes[2] = shape2;
	env->shape_count = 3;
}

void free_tile_shapes()
{
	FREE(env->shapes);
	env->shapes = 0;
	env->shape_count = 0;
}

shape_t get_transformed_tile_shape(tile_t tile)
{
	shape_t shape;
	shape.type = C2_NONE;
	if (tile.shape_id == -1) return shape;
	shape = env->shapes[tile.shape_id];

	switch (shape.type)
	{
		case C2_POLY:
		{
			c2Poly poly = shape.u.poly;
			for (int j = 0; j < poly.count; ++j)
			{
				c2v v = c2V(poly.verts[j].x * tile.sprite.sx, poly.verts[j].y * tile.sprite.sy);
				poly.verts[j] = c2V(v.x + tile.sprite.x, v.y + tile.sprite.y);
			}
			shape.u.poly = poly;
		}	break;

		case C2_AABB:
		{
			c2v a = shape.u.aabb.min;
			c2v b = shape.u.aabb.max;
			a = c2V(a.x * c2Abs(tile.sprite.sx) + tile.sprite.x, a.y * c2Abs(tile.sprite.sy) + tile.sprite.y);
			b = c2V(b.x * c2Abs(tile.sprite.sx) + tile.sprite.x, b.y * c2Abs(tile.sprite.sy) + tile.sprite.y);
			shape.u.aabb.min = a;
			shape.u.aabb.max = b;
		}	break;
	}

	return shape;
}

void debug_draw_tile_shapes(tile_t* tiles, int tile_count)
{
	void* ctx = env->ctx_gl;
	gl_line_color(ctx, 1, 0, 0);
	for (int i = 0; i < tile_count; ++i)
	{
		tile_t tile = tiles[i];
		shape_t shape = get_transformed_tile_shape(tile);
		if (shape.type == C2_NONE) continue;

		switch (shape.type)
		{
		case C2_POLY:
		{
			c2Poly poly = shape.u.poly;
			for (int j = 0; j < poly.count; ++j)
			{
				int iA = j;
				int iB = (j + 1) % poly.count;
				c2v a = poly.verts[iA];
				c2v b = poly.verts[iB];
				gl_line(ctx, a.x, a.y, 0, b.x, b.y, 0);
			}
		}	break;

		case C2_AABB:
		{
			c2v a = shape.u.aabb.min;
			c2v b = shape.u.aabb.max;
			c2v c = c2V(a.x, b.y);
			c2v d = c2V(b.x, a.y);
			gl_line(ctx, a.x, a.y, 0, c.x, c.y, 0);
			gl_line(ctx, c.x, c.y, 0, b.x, b.y, 0);
			gl_line(ctx, b.x, b.y, 0, d.x, d.y, 0);
			gl_line(ctx, d.x, d.y, 0, a.x, a.y, 0);
		}	break;
		}
	}
}

cute_tiled_map_t* load_map(const char* path)
{
	void* map_memory;
	int sz;
	if (!load_file(path, &map_memory, &sz)) return 0;
	cute_tiled_map_t* map = cute_tiled_load_map_from_memory(map_memory, sz, 0);
	FREE(map_memory);
	return map;
}

int* load_tile_map_shape_ids(const char* path, int* tile_count_out)
{
	cute_tiled_map_t* map = load_map(path);
	cute_tiled_tileset_t* tileset = map->tilesets;
	int map_width = map->width;
	int map_height = map->height;
	if (tile_count_out) *tile_count_out = map_width * map_height;
	int* shape_ids = (int*)ALLOC(sizeof(int) * map_width * map_height);

	// read the shape id from each tile
	cute_tiled_layer_t* layer = map->layers;
	int* tile_ids = layer->data;
	env->shape_count = layer->data_count;
	for (int i = 0; i < layer->data_count; ++i)
	{
		// grab and clear the flipping bits
		int hflip, vflip, dflip;
		int global_tile_id = tile_ids[i];
		cute_tiled_get_flags(global_tile_id, &hflip, &vflip, &dflip);
		global_tile_id = cute_tiled_unset_flags(global_tile_id);

		// assume the map file only has one tileset
		// resolve the tile id based on the matching tileset's first gid
		// the final index can be used on the `images` global array
		int id = global_tile_id - tileset->firstgid;

		// -1 since the first shape is "empty", or no shape at all
		shape_ids[i] = id - 1;
	}

	// assumed true for this test map
	assert(layer->data_count == map_width * map_height);

	cute_tiled_free_map(map);

	return shape_ids;
}

tile_t* load_tile_map(const char* path, int* tile_count_out, int* shape_ids, const char* image_path_base)
{
	cute_tiled_map_t* map = load_map(path);
	cute_tiled_tileset_t* tileset = map->tilesets;
	if (!map) return 0;

	int map_width = map->width;
	int map_height = map->height;

	tile_t* tiles = (tile_t*)ALLOC(sizeof(tile_t) * map_width * map_height);

	// create tiles based off of the parsed map file data
	cute_tiled_layer_t* layer = map->layers;
	int* tile_ids = layer->data;
	int tile_count = layer->data_count;
	if (tile_count_out) *tile_count_out = tile_count;

	// assumed true for this test map
	assert(tile_count == map_width * map_height);

	for (int i = 0; i < tile_count; ++i)
	{
		// grab and clear the flipping bits
		int hflip, vflip, dflip;
		int global_tile_id = tile_ids[i];
		cute_tiled_get_flags(global_tile_id, &hflip, &vflip, &dflip);
		global_tile_id = cute_tiled_unset_flags(global_tile_id);

		// assume the map file only has one tileset
		// resolve the tile id based on the matching tileset's first gid
		// the final index can be used on the `images` global array
		int id = global_tile_id - tileset->firstgid;

		// Special case handling for "no tile".
		if (id < 0)
		{
			id = 0;
		}

		int x = i % map_width;
		int y = map_height - i / map_width;

		char buf[256];
		sprintf(buf, "%s%d.png", image_path_base, id + 1);

		sprite_t sprite = get_sprite(buf);
		sprite.depth = 0;
		sprite.x = (float)x;
		sprite.y = (float)y;

		// Handle flip flags by enumerating all bit possibilities.
		// Three bits means 2^3 = 8 possible sets. This is to support the
		// flip and rotate tile buttons in Tiled.
		int flags = dflip << 0 | vflip << 1 | hflip << 2;

		// just hard-code the sin/cos values directly since they are all factors of 90 degrees
#define ROTATE_90_CCW(sp) do { sp.c = 0.0f; sp.s = 1.0f; } while (0)
#define ROTATE_90_CW(sp) do { sp.c = 0.0f; sp.s = -1.0f; } while (0)
#define ROTATE_180(sp) do { sp.c = -1.0f; sp.s = 0.0f; } while (0)
#define FLIP_VERTICAL(sp) sp.sy *= -1.0f
#define FLIP_HORIZONTAL(sp) sp.sx *= -1.0f

		sprite.c = 1.0f;
		sprite.s = 0.0f;

		// [hflip, vflip, dflip]
		switch (flags)
		{
		case 1: // 001
			ROTATE_90_CCW(sprite);
			FLIP_VERTICAL(sprite);
			break;

		case 2: // 010
			FLIP_VERTICAL(sprite);
			break;

		case 3: // 011
			ROTATE_90_CCW(sprite);
			break;

		case 4: // 100
			FLIP_HORIZONTAL(sprite);
			break;

		case 5: // 101
			ROTATE_90_CW(sprite);
			break;

		case 6: // 110
			ROTATE_180(sprite);
			break;

		case 7: // 111
			ROTATE_90_CCW(sprite);
			FLIP_HORIZONTAL(sprite);
			break;
		}

		// draw tiles from bottom left corner of each tile
		sprite.x += 0.5f;
		sprite.y -= 0.5f;

		// center all tiles onto the screen
		sprite.x -= (float)(map_width / 2);
		sprite.y -= (float)(map_height / 2);

		// scale by tile size
		sprite.x *= sprite.image_w;
		sprite.y *= sprite.image_h;

		// scale by factor of two (source pixels are 2x2 pixel blocks on screen)
		sprite.x *= 2.0f;
		sprite.y *= 2.0f;
		sprite.sx *= 2.0f;
		sprite.sy *= 2.0f;

		// construct the tile sprite and (x, y) position
		tile_t tile;
		tile.sprite = sprite;
		tile.x = x;
		tile.y = y;
		tile.shape_id = shape_ids[id];

		tiles[i] = tile;
	}

	cute_tiled_free_map(map);

	return tiles;
}

void init_tile_system()
{
	init_tile_shapes();
}

void shutdown_tile_system()
{
	free_tile_shapes();
}
