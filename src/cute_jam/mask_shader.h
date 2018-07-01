
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
	gl_load_shader(&env->mask_shader, vs, ps);
	gl_set_shader(&env->mask_renderable, &env->sprite_shader);

	gl_send_matrix(&env->sprite_shader, "u_mvp", env->projection);
}