#pragma once

struct dog_t : public entity_t
{
	dog_t() { id = EID_DOG;  }
	sprite_t dog_head_sprite;
	sprite_t* dog_bod_sprites;
	sprite_t dog_bootay_sprite;
	int num_sections;
};

void create_dog()
{
	dog_t* dog = NEW(dog_t);
	dog->dog_head_sprite = get_sprite("/data/cavestory/sprites/dog_head.png");
	dog->num_sections = rand() % 4 + 2;

	dog->dog_bod_sprites = malloc(dog->num_sections * sizeof(sprite_t));

	for (int i = 0; i < dog->num_sections; i++) 
	{
		dog->dog_bod_sprite[i] = get_sprite("/data/cavestory/sprites/dog_bod.png");
	}
	dog->dog_bootay_sprite = get_sprite("/data/cavestory/sprites/dog_bootay.png");
	//dog->dog_sprite.x = rand() % 15;
	add_entity_to_list(&env->entity_list, dog);
}

void update_dog(entity_t* entity, float dt)
{
	dog_t* dog = (dog_t*)entity;
	dog->dog_sprite.y = sin(env->game_time * 10) * 5;
}

void draw_dog(entity_t* entity)
{
	dog_t* dog = (dog_t*)entity;

	push_sprite(dog->dog_sprite);
}