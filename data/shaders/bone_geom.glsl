#version 420 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout(location=0) in vec4 v_pos2[];
layout(location=1) in vec4 v_highlight[];
layout(location=2) in vec4 v_color[];
//layout(location=3) in mat4 v_transform[];

uniform mat4 mvp;
uniform vec2 screensize;

out vec4 g_color;
out vec4 g_highlight;

void main() {
	vec4 delta = v_pos2[0]-gl_in[0].gl_Position;
	//
	gl_Position = mvp * (gl_in[0].gl_Position + vec4(0,v_highlight[0].x,0,0));
	g_color = v_color[0];
	g_highlight = v_highlight[0];
	EmitVertex();

	gl_Position = mvp * (v_pos2[0] + vec4(0,v_highlight[0].y,0,0));
	g_color = v_color[0];
	g_highlight = v_highlight[0];
	EmitVertex();

	gl_Position = mvp * (gl_in[0].gl_Position + vec4(0,-v_highlight[0].x,0,0));
	g_color = v_color[0];
	g_highlight = v_highlight[0];
	EmitVertex();


	gl_Position = mvp * (v_pos2[0] + vec4(0,-v_highlight[0].y,0,0));
	g_color = v_color[0];
	g_highlight = v_highlight[0];
	EmitVertex();

	EndPrimitive();
}
