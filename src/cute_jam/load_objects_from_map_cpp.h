void load_objects_from_map(cute_tiled_object_t* object, float map_width, float map_height, float tile_size)
{
	map_width *= tile_size;
	map_height *= tile_size;

	while (object)
	{
		float x, y;
		x = object->x - map_width / 2.0f;
		y = (map_height - object->y) - (map_height / 2.0f);
		create_coin(x * 2.0f, y * 2.0f);
		object = object->next;
	}
}
