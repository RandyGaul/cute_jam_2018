
void setup_mask_shader()
{

#define STR(x) #x

	const char* vs = STR(
		#version 300 es\n

		uniform mat4 u_mvp;

		in vec2 in_pos;
		in vec2 in_uv;

		out vec2 v_uv;

		void main()
		{
			v_uv = in_uv;
			gl_Position = u_mvp * vec4(in_pos, 0, 1);
		}
	);

	const char* ps = STR(
		#version 300 es\n
		precision mediump float;

		uniform float u_cutoff;
		uniform vec4 u_color;

		in vec2 v_uv;
		out vec4 out_col;

		void main()
		{
			float c = u_cutoff < v_uv.x ? 1.0f : 0.0f;
			out_col = u_color * c;
		}
	);

	gl_vertex_data_t vd;
	gl_make_vertex_data(&vd, 1024 * 1024, GL_TRIANGLES, sizeof(vertex_t), GL_DYNAMIC_DRAW);
	gl_add_attribute(&vd, "in_pos", 2, CUTE_GL_FLOAT, CUTE_GL_OFFSET_OF(vertex_t, pos));
	gl_add_attribute(&vd, "in_uv", 2, CUTE_GL_FLOAT, CUTE_GL_OFFSET_OF(vertex_t, uv));

	gl_make_renderable(&env->mask_renderable, &vd);
	gl_load_shader(&env->mask_shader, vs, ps);
	gl_set_shader(&env->mask_renderable, &env->mask_shader);

	gl_send_matrix(&env->mask_shader, "u_mvp", env->projection);
}

void draw_meter(vertex_t* verts, int x, int y, int w, int h, float* color, float cutoff)
{
	make_sprite_quad(x, y, w, h, verts);
	gl_draw_call_t call;
	call.r = &env->mask_renderable;
	call.textures[0] = 0;
	call.texture_count = 1;
	call.verts = verts;
	call.vert_count = 6;
	gl_send_f32(call.r->program, "u_cutoff", 1, &cutoff, 1);
	gl_send_f32(call.r->program, "u_color", 1, color, 4);
	gl_push_draw_call(env->ctx_gl, call);
}

void update_mask_shader()
{
	const int meter_width = 50;
	const int meter_height = 10;
	const int vertical_offset = -25;
	const int border = 2;
/*
	float white[] = { 1,1,1,1 };
	static vertex_t border_verts[6];
	draw_meter(border_verts, env->player->quote_sprite.x, env->player->quote_sprite.y + vertical_offset, meter_width + border*2, meter_height + border*2, white, 0);*/
	float red[] = { 1,0,0,1 };
	static vertex_t meter_verts[6];
	draw_meter(meter_verts, env->player->quote_sprite.x, env->player->quote_sprite.y + vertical_offset, meter_width, meter_height, red, env->playa->jetpackCD);
}