#version 420 core

layout (lines) in;
layout (line_strip, max_vertices = 21) out;

layout(location=0) in vec4 v_guides[];
layout(location=1) in vec4 v_highlight[];
layout(location=2) in vec4 v_color[];

uniform vec2 screensize;

out vec4 g_highlight;
out vec4 g_color;

vec2 bezier(float t, vec2 as, vec2 ap, vec2 bs, vec2 bp) {
	float mt = 1-t;
	return (mt*mt*mt*as)+(3*mt*mt*t*ap)+(3*mt*t*t*bp)+(t*t*t*bs);
}

void main() {
	vec2 as = gl_in[0].gl_Position.xy;
	vec2 ap = v_guides[0].zw; //out guide for A
	vec2 bs = gl_in[1].gl_Position.xy;
	vec2 bp = v_guides[1].xy; //in guide for B

	int prec = 20;
	for(int i = 0 ; i <= prec; ++i) {
		float ratio = (1.0*i)/prec;
		gl_Position = vec4(bezier(ratio,as,ap,bs,bp),0,1);

		g_color = v_color[0];
		g_highlight = v_highlight[0];
		EmitVertex();
	}

	EndPrimitive();
}
