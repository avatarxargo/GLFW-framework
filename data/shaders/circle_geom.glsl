#version 420 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout(location=0) in vec2 v_rad[];
layout(location=1) in vec4 v_params[];
layout(location=2) in vec4 v_color[];

uniform vec2 screensize;

out vec2 g_texcoord;
out vec2 g_rad;
out vec4 g_color;
out vec4 g_params;

void main() {
	vec2 off = 2*vec2(v_rad[0].x,v_rad[0].x)/screensize;

	gl_Position = gl_in[0].gl_Position + vec4(-off.x, -off.y, 0.0, 0.0);
	g_texcoord = vec2(-1,-1);
	g_rad = v_rad[0];
	g_color = v_color[0];
	g_params = v_params[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(-off.x, off.y, 0.0, 0.0);
	g_texcoord = vec2(-1,1);
	g_rad = v_rad[0];
	g_color = v_color[0];
	g_params = v_params[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(off.x, -off.y, 0.0, 0.0);
	g_texcoord = vec2(1,-1);
	g_rad = v_rad[0];
	g_color = v_color[0];
	g_params = v_params[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(off.x, off.y, 0.0, 0.0);
	g_texcoord = vec2(1,1);
	g_rad = v_rad[0];
	g_color = v_color[0];
	g_params = v_params[0];
	EmitVertex();

	EndPrimitive();
}
