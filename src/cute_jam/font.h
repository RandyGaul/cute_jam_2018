void draw_text(font_t* font, const char* text, float x, float y, float line_height);
font_t* load_bmfont(const char* font_path, const char* image_path);
void init_font_system();
void shutdown_font_system();

//--------------------------------------------------------------------------------------------------

font_t* load_bmfont(const char* font_path, const char* image_path)
{
	int font_sz;
	void* font_data;
	if (!load_file(font_path, &font_data, &font_sz)) return 0;

	int img_sz;
	void* img_data;
	if (!load_file(image_path, &img_data, &img_sz))
	{
		FREE(font_data);
		return 0;
	}

	cp_image_t img = cp_load_png_mem(img_data, img_sz);
	CUTE_FONT_U64 texture_handle = generate_texture_handle(img.pix, img.w, img.h);
	font_t* font = cute_font_load_bmfont(texture_handle, font_data, font_sz, 0);
	cp_free_png(&img);

	FREE(font_data);
	FREE(img_data);

	return font;
}

void draw_text(font_t* font, const char* text, float x, float y, float line_height)
{
	int vert_count;

	while (1)
	{
		float w = (float)cute_font_text_width(font, text);
		float h = (float)cute_font_text_height(font, text);
		cute_font_fill_vertex_buffer(font, text, x + -w / 2, y + h / 2, line_height, env->font_verts + env->font_verts_count, env->font_verts_capacity - env->font_verts_count, &vert_count);

		if (vert_count != env->font_verts_capacity - env->font_verts_count)
		{
			break;
		}

		int cap = env->font_verts_capacity ? env->font_verts_capacity * 2 : 1024;
		BUFFER_GROW(env, font_verts_count, font_verts_capacity, font_verts, cute_font_vert_t, cap);
	}

	gl_draw_call_t call;
	call.textures[0] = (uint32_t)font->atlas_id;
	call.texture_count = 1;
	call.r = &env->sprite_renderable;
	call.verts = env->font_verts + env->font_verts_count;
	call.vert_count = vert_count;

	gl_push_draw_call(env->ctx_gl, call);

	env->font_verts_count += vert_count;
}

void init_font_system()
{
	env->font_verts_capacity = 1024;
	env->font_verts = (cute_font_vert_t*)ALLOC(sizeof(vertex_t) * env->font_verts_capacity);
	env->courier_new = load_bmfont("/data/fonts/courier_new.fnt", "/data/fonts/courier_new_0.png");
}

void shutdown_font_system()
{
	FREE(env->font_verts);
	env->font_verts = 0;
	env->font_verts_capacity = 0;
	cute_font_free(env->courier_new);
	env->courier_new = 0;
}
