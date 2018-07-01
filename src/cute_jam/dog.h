#pragma once

#define DOG_VISION_RANGE 100
#define DOG_MOVEMENT_SPEED 30
#define DOG_ROTATION_SPEED 3

struct dog_t : public entity_t
{
	dog_t() { id = EID_DOG;  }
	sprite_t dog_head_sprite;
	sprite_t* dog_bod_sprites;
	sprite_t dog_bootay_sprite;

	v2 pos;
	v2 vel;
	int num_sections;
	float angle;
};

void update_sprite_positions(dog_t* dog);
void update_sprite_rotations(dog_t* dog);
void collide(dog_t* dawg);

void create_dog()
{
	dog_t* dog = NEW(dog_t);
	dog->dog_head_sprite = get_sprite("/data/cavestory/sprites/dog_head.png");
	dog->num_sections = 3;
	dog->pos.x = rand() % 15 + 2;
	dog->pos.y = 10;
	dog->angle = 45;
	dog->dog_head_sprite.sx *= 2;
	dog->dog_head_sprite.sy *= 2;
	dog->vel.x = 0;
	dog->vel.y = 0;

	dog->dog_bod_sprites = (sprite_t*)ALLOC(dog->num_sections * sizeof(sprite_t));

	for (int i = 0; i < dog->num_sections; i++) 
	{
		dog->dog_bod_sprites[i] = get_sprite("/data/cavestory/sprites/dog_bod.png");
		dog->dog_bod_sprites[i].sx *= 2;
		dog->dog_bod_sprites[i].sy *= 2;
	}

	dog->dog_bootay_sprite = get_sprite("/data/cavestory/sprites/dog_bootay.png");
	dog->dog_bootay_sprite.sx *= 2;
	dog->dog_bootay_sprite.sy *= 2;

	update_sprite_positions(dog);
	update_sprite_rotations(dog);

	add_entity_to_list(&env->entity_list, dog);
}

void collide(dog_t* dawg)
{
	float half_bod_length = (dawg->num_sections * dawg->dog_bod_sprites[0].sx) / 2;

	c2v ll{ -half_bod_length - dawg->dog_head_sprite.sx / 2,
		-dawg->dog_head_sprite.sy / 2 };
	c2v ur{ half_bod_length + dawg->dog_bootay_sprite.sx / 2,
		dawg->dog_bootay_sprite.sy / 2 };
	c2AABB bb{ ll, ur};

	c2Poly poly;
	poly.count = 4;

	c2BBVerts(poly.verts, &bb);

	rotation rot{ sinf(dawg->angle), cosf(dawg->angle) };
	transform trans{ rot, dawg->pos };

	for (int i = 0; i < poly.count; i++) {
		v2 transformedVert = mul(trans, v2(poly.verts[i].x, poly.verts[i].y));
		poly.verts[i] = { transformedVert.x, transformedVert.y };
	}

	c2Manifold man;
	c2Collide(&poly, 0, C2_POLY, &env->playa->quote_circle, 0, C2_CIRCLE, &man);

	gl_line_color(env->ctx_gl, 1, 0, 0);

	for (int i = 0; i < poly.count; i++) 
	{
		int nextIndex = i == poly.count - 1 ? 0 : i + 1;
		gl_line(env->ctx_gl, poly.verts[i].x, poly.verts[i].y, 0, poly.verts[nextIndex].x, poly.verts[nextIndex].y, 0);
	}
}

void update_sprite_positions(dog_t* dog) 
{
	float half_length = (dog->num_sections * dog->dog_bod_sprites[0].sx + dog->dog_head_sprite.sx + dog->dog_bootay_sprite.sx) / 2;

	dog->dog_head_sprite.x = dog->pos.x - (half_length - dog->dog_head_sprite.sx / 2) * cosf(dog->angle);
	dog->dog_head_sprite.y = dog->pos.y - (half_length - dog->dog_head_sprite.sx / 2) * sinf(dog->angle);

	for (int i = 0; i < dog->num_sections; i++)
	{
		dog->dog_bod_sprites[i].x = dog->dog_head_sprite.x + (i + 1) * dog->dog_bod_sprites[i].sx * cosf(dog->angle);
		dog->dog_bod_sprites[i].y = dog->dog_head_sprite.y + (i + 1) * dog->dog_bod_sprites[i].sx * sinf(dog->angle);
	}

	dog->dog_bootay_sprite.x = dog->pos.x + (half_length - dog->dog_bootay_sprite.sx / 2) * cosf(dog->angle);
	dog->dog_bootay_sprite.y = dog->pos.y + (half_length - dog->dog_bootay_sprite.sx / 2) * sinf(dog->angle);
}

void update_sprite_rotations(dog_t* dog)
{
	dog->dog_head_sprite.c = cosf(dog->angle);
	dog->dog_head_sprite.s = sinf(dog->angle);

	for (int i = 0; i < dog->num_sections; i++)
	{
		dog->dog_bod_sprites[i].c = cosf(dog->angle);
		dog->dog_bod_sprites[i].s = sinf(dog->angle);
	}

	dog->dog_bootay_sprite.c = cosf(dog->angle);
	dog->dog_bootay_sprite.s = sinf(dog->angle);
}

void dog_intelligence(dog_t* dog, float dt)
{
	if (distance(dog->pos, v2(env->playa->quote_x, env->playa->quote_y)) < DOG_VISION_RANGE)
	{
		v2 facing = norm(v2(dog->dog_head_sprite.x, dog->dog_head_sprite.y) - v2(dog->dog_bootay_sprite.x, dog->dog_bootay_sprite.y));
		v2 toPlayer = norm(v2(env->playa->quote_x, env->playa->quote_y) - dog->pos);
		if (acos(dot(facing, toPlayer)) < .05)
		{
			// charge towards player
			dog->vel = toPlayer * DOG_MOVEMENT_SPEED;
		}
		else {
			int dir = (facing.x * toPlayer.y - facing.y * toPlayer.x) > 0 ? 1 : -1;
			dog->angle += dt * dir * DOG_ROTATION_SPEED;
		}
	}
}

void update_dog(entity_t* entity, float dt)
{
	dog_t* dog = (dog_t*)entity;

	dog->pos += dog->vel * dt;

	dog_intelligence(dog, dt);
	update_sprite_rotations(dog);
	update_sprite_positions(dog);

	collide(dog);
}

void draw_dog(entity_t* entity)
{
	dog_t* dog = (dog_t*)entity;

	for (int i = 0; i < dog->num_sections; i++)
	{
		push_sprite(dog->dog_bod_sprites[i]);
	}

	push_sprite(dog->dog_bootay_sprite);
	push_sprite(dog->dog_head_sprite);
}