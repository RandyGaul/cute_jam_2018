enum entity_id_t
{
	EID_NONE,
	EID_PLAYER,
	EID_COIN,
	// Add more types here.

	EID_COUNT
};

struct entity_t
{
	entity_id_t id = EID_NONE;
	entity_t* next = this;
	entity_t* prev = this;
};

void update_entities(entity_t* e, float dt);
void draw_entities(entity_t* e);
