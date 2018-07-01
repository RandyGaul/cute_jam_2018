#pragma once

struct dog_t : public entity_t
{
	dog_t() { id = EID_DOG;  }
	sprite_t dog_sprite;
};

void create_dog()
{
	dog_t* dog = NEW(dog_t);
	dog->dog_sprite = get_sprite("/data/cavestory/sprites/dog_head.png");
	add_entity_to_list(&env->entity_list, dog);
}

void update_dog(entity_t* entity, float dt)
{
	dog_t* dog = (dog_t*)entity;
	dog->dog_sprite.x = 0;
	dog->dog_sprite.y = sin(env->game_time * 10) * 5;
}

void draw_dog(entity_t* entity)
{
	dog_t* dog = (dog_t*)entity;

	push_sprite(dog->dog_sprite);
}