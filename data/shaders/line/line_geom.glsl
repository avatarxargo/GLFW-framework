#version 420 core

layout (lines) in;
layout (line_strip, max_vertices = 3) out;

layout(location=0) in vec4 v_highlight[];
layout(location=1) in vec4 v_color[];

layout(location=0) out vec4 g_highlight;
layout(location=1) out vec4 g_color;

void main() {
	
	gl_Position = gl_in[0].gl_Position;
	g_color = v_color[0];
	g_highlight = v_highlight[0];
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position;
	g_color = v_color[1];
	g_highlight = v_highlight[1];
	EmitVertex();

	EndPrimitive();
}
