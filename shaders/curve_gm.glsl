#version 420 core

layout (lines) in;
layout (triangle_strip, max_vertices = 10) out;

in vec2 tex_coord[];
in vec3 position[];
in vec3 pospos[];
in vec3 normal[];
in vec3 camnormal[];

uniform vec2 screensize;
uniform mat4 P;

out vec2 tex_coord_g;

void main() {

	vec4 pa = P*gl_in[0].gl_Position;
	vec4 pb = P*gl_in[1].gl_Position;
			vec4 along = pb-pa;
			pa -= along*0.1;
			pb += along*0.1;
		vec4 na = P*vec4(camnormal[0],0);
		vec4 nb = P*vec4(camnormal[1],0);
	float scl = 0.005	;
	float ascl = -scl;
	float ztilt = 0;//-0.005;

	gl_Position = pa+ascl*na+vec4(0,0,ztilt,0);
	tex_coord_g = vec2(0,0);
	EmitVertex();
	gl_Position = pa+scl*na+vec4(0,0,ztilt,0);
	tex_coord_g = vec2(0,1);
	EmitVertex();
	gl_Position = pb+ascl*na+vec4(0,0,ztilt,0);
	tex_coord_g = vec2(1,0);
	EmitVertex();
	gl_Position = pb+scl*na+vec4(0,0,ztilt,0);
	tex_coord_g = vec2(1,1);
	EmitVertex();

	EndPrimitive();
}
