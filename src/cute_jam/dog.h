#pragma once

struct dog_t : public entity_t
{
	dog_t() { id = EID_DOG;  }
	sprite_t dog_head_sprite;
	sprite_t* dog_bod_sprites;
	sprite_t dog_bootay_sprite;

	v2 pos;
	int num_sections;
	float angle;
};

void update_sprite_positions(dog_t* dog);
void update_sprite_rotations(dog_t* dog);

void create_dog()
{
	dog_t* dog = NEW(dog_t);
	dog->dog_head_sprite = get_sprite("/data/cavestory/sprites/dog_head.png");
	dog->num_sections = 3;
	dog->pos.x = rand() % 15 + 2;
	dog->pos.y = 10;
	dog->angle = 45;

	dog->dog_bod_sprites = (sprite_t*)ALLOC(dog->num_sections * sizeof(sprite_t));

	for (int i = 0; i < dog->num_sections; i++) 
	{
		dog->dog_bod_sprites[i] = get_sprite("/data/cavestory/sprites/dog_bod.png");
	}

	dog->dog_bootay_sprite = get_sprite("/data/cavestory/sprites/dog_bootay.png");

	update_sprite_positions(dog);
	update_sprite_rotations(dog);

	add_entity_to_list(&env->entity_list, dog);
}

void update_sprite_positions(dog_t* dog) 
{
	float half_length = (dog->num_sections * dog->dog_bod_sprites[0].sx) / 2;

	dog->dog_head_sprite.x = dog->pos.x - (half_length + dog->dog_head_sprite.sx / 2) * cosf(dog->angle);
	dog->dog_head_sprite.y = dog->pos.y - (half_length + dog->dog_head_sprite.sx / 2) * sinf(dog->angle);

	for (int i = 0; i < dog->num_sections; i++)
	{
		dog->dog_bod_sprites[i].x = dog->dog_head_sprite.x + (i + 1) * dog->dog_bod_sprites[i].sx * cosf(dog->angle);
		dog->dog_bod_sprites[i].y = dog->dog_head_sprite.y + (i + 1) * dog->dog_bod_sprites[i].sx * sinf(dog->angle);
	}

	dog->dog_bootay_sprite.x = dog->pos.x + (half_length + dog->dog_bootay_sprite.sx / 2) * cosf(dog->angle);
	dog->dog_bootay_sprite.y = dog->pos.y + (half_length + dog->dog_bootay_sprite.sx / 2) * sinf(dog->angle);
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

void update_dog(entity_t* entity, float dt)
{
	dog_t* dog = (dog_t*)entity;

	//dog->pos.y = sin(env->game_time * 3) * 10 + 10;
	dog->angle += dt * 3;

	update_sprite_rotations(dog);
	update_sprite_positions(dog);
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