struct vtable_t
{
	void (*update)(entity_t*, float);
	void (*draw)(entity_t*);
};

#define VTABLE_ENTRY(T) \
	{ \
		(void (*)(entity_t*, float))update_##T, \
		(void (*)(entity_t*))draw_##T \
	}

vtable_t g_vtable[] = {
	VTABLE_ENTRY(player),
	VTABLE_ENTRY(coin),
};

void update_entities(entity_t* entity_list, float dt)
{
	if (!entity_list) return;
	entity_t* entity = entity_list;
	do
	{
		int id = entity->id - 1;
		assert(id >= 0 && id < ARRAY_SIZE(g_vtable));
		g_vtable[id].update(entity, dt);
		entity = entity->next;
	}
	while (entity != entity_list);
}

void draw_entities(entity_t* entity_list)
{
	if (!entity_list) return;
	entity_t* entity = entity_list;
	do
	{
		int id = entity->id - 1;
		assert(id >= 0 && id < ARRAY_SIZE(g_vtable));
		g_vtable[id].draw(entity);
		entity = entity->next;
	}
	while (entity != entity_list);
}

void add_entity_to_list(entity_t** list_ptr, entity_t* entity)
{
	if (*list_ptr)
	{
		DLIST_INSERT(*list_ptr, entity);
	}

	else
	{
		*list_ptr = entity;
	}
}

void remove_entity(entity_t* entity)
{
	DLIST_REMOVE(entity);
}
