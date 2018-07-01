struct coin_t;
void set_coin_position(coin_t* coin, int x, int y);
const int COIN_ID = 0;
const int TILE_SIZE = 16;

struct coin_t : public entity_t
{
	coin_t() { id = EID_COIN; }
	sprite_t sprite;
	c2Circle circle;
};

coin_t* create_coin(int x = 0, int y = 0)
{
	coin_t* coin = NEW(coin_t);
	coin->sprite = get_sprite("/data/sprites/coin.png");
	coin->sprite.sx *= 2;
	coin->sprite.sy *= 2;
	coin->circle.r = 15;
	set_coin_position(coin, x, y);
	add_entity_to_list(&env->entity_list, coin);
	return coin;
}

void grab_coin(coin_t*& coin)
{
	play_sound("/data/sounds/coin.wav");
	play_sound("/data/sounds/meow.wav");
	env->coin_count++;
	remove_entity(coin);
	coin = nullptr;
}

void set_coin_position(coin_t* coin, int x, int y)
{
	coin->circle.p.x = x;
	coin->circle.p.y = y;
	coin->sprite.x = x;
	coin->sprite.y = y;
}

void update_coin(entity_t* entity, float dt)
{
	coin_t* coin = (coin_t*)entity;
	c2Circle& playerCircle = env->player->quote_circle;
	c2Manifold m;
	c2Collide(&coin->circle, 0, C2_CIRCLE, &playerCircle, 0, C2_CIRCLE, &m);

	if (m.count)
	{
		grab_coin(coin);
	}
}

void draw_coin(entity_t* entity)
{
	coin_t* coin = (coin_t*)entity;
	push_sprite(coin->sprite);
}