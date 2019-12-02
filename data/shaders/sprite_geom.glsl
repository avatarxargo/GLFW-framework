#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 sizeg[];
in vec2 texidg[];
in vec2 uvstartg[];
in vec2 uvdimg[];
in float v_angle[];
in vec4 v_color[];

uniform mat4 mvp;

out vec2 textureCoord;
out vec2 texid;
out vec4 g_color;

void main() {
	texid = texidg[0];
	mat4 m;
	m[0] = vec4(cos(v_angle[0]),-sin(v_angle[0]),0,0);
	m[1] = vec4(sin(v_angle[0]),cos(v_angle[0]),0,0);
	m[2] = vec4(0,0,1,0);
	m[3] = vec4(0,0,0,1);
	gl_Position = mvp * (gl_in[0].gl_Position + m * vec4(-sizeg[0].x/2, -sizeg[0].y/2, 0.0, 0.0));
	textureCoord = uvstartg[0];
	g_color = v_color[0];
	EmitVertex();
	gl_Position = mvp * (gl_in[0].gl_Position + m * vec4(-sizeg[0].x/2, sizeg[0].y/2, 0.0, 0.0));
	textureCoord = uvstartg[0] + vec2(0,uvdimg[0].y);
	g_color = v_color[0];
	EmitVertex();
	gl_Position = mvp *  (gl_in[0].gl_Position + m * vec4(sizeg[0].x/2, -sizeg[0].y/2, 0.0, 0.0));
	textureCoord = uvstartg[0] + vec2(uvdimg[0].x,0);
	g_color = v_color[0];
	EmitVertex();
	gl_Position = mvp *(gl_in[0].gl_Position +  m * vec4(sizeg[0].x/2, sizeg[0].y/2, 0.0, 0.0));
	textureCoord = uvstartg[0] + uvdimg[0];
	g_color = v_color[0];
	EmitVertex();
	EndPrimitive();
}
