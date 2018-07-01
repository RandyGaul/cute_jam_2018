struct vertex_t
{
	v2 pos;
	v2 uv;
};

void init_graphics(int width_in_pixels, int height_in_pixels);
void shutdown_graphics();
SPRITEBATCH_U64 generate_texture_handle(void* pixels, int w, int h);
void destroy_texture_handle(SPRITEBATCH_U64 texture_id);
void make_sprite_quad(float x, float y, float sx, float sy, vertex_t* out);

//--------------------------------------------------------------------------------------------------

// callbacks for tinyspritebatch.h
void batch_report(spritebatch_sprite_t* sprites, int count)
{
	// build the draw call
	gl_draw_call_t call;
	call.r = &env->sprite_renderable;
	call.textures[0] = (uint32_t)sprites[0].texture_id;
	call.texture_count = 1;

	// set texture uniform in shader
	gl_send_texture(call.r->program, "u_sprite_texture", 0);

	// NOTE:
	// perform any additional sorting here

	if (env->sprite_verts_capacity < env->sprite_verts_count + count * 6)
	{
		BUFFER_GROW(env, sprite_verts_count, sprite_verts_capacity, sprite_verts, vertex_t, env->sprite_verts_count + count * 6);
	}

	// build vertex buffer of quads from all sprite transforms
	call.verts = env->sprite_verts + env->sprite_verts_count;
	call.vert_count = count * 6;
	env->sprite_verts_count += call.vert_count;

	vertex_t* verts = (vertex_t*)call.verts;
	for (int i = 0; i < count; ++i)
	{
		spritebatch_sprite_t* s = sprites + i;

		typedef struct v2
		{
			float x;
			float y;
		} v2;

		v2 quad[] = {
			{ -0.5f,  0.5f },
			{  0.5f,  0.5f },
			{  0.5f, -0.5f },
			{ -0.5f, -0.5f },
		};

		for (int j = 0; j < 4; ++j)
		{
			float x = quad[j].x;
			float y = quad[j].y;

			// rotate sprite about origin
			float x0 = s->c * x - s->s * y;
			float y0 = s->s * x + s->c * y;
			x = x0;
			y = y0;

			// scale sprite about origin
			x *= s->sx;
			y *= s->sy;

			// translate sprite into the world
			x += s->x;
			y += s->y;

			quad[j].x = x;
			quad[j].y = y;
		}

		// output transformed quad into CPU buffer
		vertex_t* out_verts = verts + i * 6;

		out_verts[0].pos.x = quad[0].x;
		out_verts[0].pos.y = quad[0].y;
		out_verts[0].uv.x = s->minx;
		out_verts[0].uv.y = s->maxy;

		out_verts[1].pos.x = quad[3].x;
		out_verts[1].pos.y = quad[3].y;
		out_verts[1].uv.x = s->minx;
		out_verts[1].uv.y = s->miny;

		out_verts[2].pos.x = quad[1].x;
		out_verts[2].pos.y = quad[1].y;
		out_verts[2].uv.x = s->maxx;
		out_verts[2].uv.y = s->maxy;

		out_verts[3].pos.x = quad[1].x;
		out_verts[3].pos.y = quad[1].y;
		out_verts[3].uv.x = s->maxx;
		out_verts[3].uv.y = s->maxy;

		out_verts[4].pos.x = quad[3].x;
		out_verts[4].pos.y = quad[3].y;
		out_verts[4].uv.x = s->minx;
		out_verts[4].uv.y = s->miny;

		out_verts[5].pos.x = quad[2].x;
		out_verts[5].pos.y = quad[2].y;
		out_verts[5].uv.x = s->maxx;
		out_verts[5].uv.y = s->miny;
	}

	// submit call to tinygl (does not get flushed to screen until `tgFlush` is called)
	gl_push_draw_call(env->ctx_gl, call);
}

void* get_pixels(SPRITEBATCH_U64 image_id)
{
	return *(void**)hashtable_find(&env->pixel_table, image_id);
}

SPRITEBATCH_U64 generate_texture_handle(void* pixels, int w, int h)
{
	GLuint location;
	glGenTextures(1, &location);
	glBindTexture(GL_TEXTURE_2D, location);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (SPRITEBATCH_U64)location;
}

void destroy_texture_handle(SPRITEBATCH_U64 texture_id)
{
	GLuint id = (GLuint)texture_id;
	glDeleteTextures(1, &id);
}

void make_sprite_quad(float x, float y, float sx, float sy, vertex_t* out)
{
	out[0] = { v2(-0.5f, 0.5f), v2(0, 1) };
	out[1] = { v2(-0.5f, -0.5f), v2(0, 0) };
	out[2] = { v2(0.5f, 0.5f), v2(1, 1) };

	out[3] = { v2(0.5f, 0.5f), v2(1, 1) };
	out[4] = { v2(-0.5f, -0.5f), v2(0, 0) };
	out[5] = { v2(0.5f, -0.5f), v2(1, 0) };
}

void setup_cute_gl(int width_in_pixels, int height_in_pixels)
{
	int clear_bits = GL_COLOR_BUFFER_BIT;
	int settings_bits = 0;
	env->ctx_gl = gl_make_ctx(1024, clear_bits, settings_bits);

#define STR(x) #x

	const char* vs = STR(
		#version 300 es\n

		uniform mat4 u_mvp;

		in vec2 in_pos;
		in vec2 in_uv;

		out vec2 v_uv;

		void main( )
		{
			v_uv = in_uv;
			gl_Position = u_mvp * vec4(in_pos, 0, 1);
		}
	);

	const char* ps = STR(
		#version 300 es\n
		precision mediump float;
	
		uniform sampler2D u_sprite_texture;

		in vec2 v_uv;
		out vec4 out_col;

		void main()
		{
			out_col = texture(u_sprite_texture, v_uv);
		}
	);

	gl_vertex_data_t vd;
	gl_make_vertex_data(&vd, 1024 * 1024, GL_TRIANGLES, sizeof(vertex_t), GL_DYNAMIC_DRAW);
	gl_add_attribute(&vd, "in_pos", 2, CUTE_GL_FLOAT, CUTE_GL_OFFSET_OF(vertex_t, pos));
	gl_add_attribute(&vd, "in_uv", 2, CUTE_GL_FLOAT, CUTE_GL_OFFSET_OF(vertex_t, uv));

	gl_make_renderable(&env->sprite_renderable, &vd);
	gl_load_shader(&env->sprite_shader, vs, ps);
	gl_set_shader(&env->sprite_renderable, &env->sprite_shader);
	
	gl_ortho_2d((float)width_in_pixels, (float)height_in_pixels, 0, 0, env->projection);
	glViewport(0, 0, width_in_pixels, height_in_pixels);

	gl_send_matrix(&env->sprite_shader, "u_mvp", env->projection);
	gl_line_mvp(env->ctx_gl, env->projection);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void swap_buffers()
{
	SDL_GL_SwapWindow(env->window);
}

void init_graphics(int width_in_pixels, int height_in_pixels)
{
	setup_cute_gl(width_in_pixels, height_in_pixels);
}

void shutdown_cute_gl()
{
	gl_free_ctx(env->ctx_gl);
}

void shutdown_graphics()
{
	shutdown_cute_gl();
}
