#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location=0) in vec2 position;
layout(location=1) in vec2 size;
layout(location=2) in vec2 uvstart;
layout(location=3) in vec2 uvdim;
layout(location=4) in float texid;
layout(location=5) in float highlight;
layout(location=6) in float angle;
layout(location=7) in vec4 color;

out vec2 sizeg;
out vec2 texidg;
out vec2 uvstartg;
out vec2 uvdimg;
out float v_angle;
out vec4 v_color;

layout(location=0) uniform mat4 mvp;
layout(location=1) uniform vec2 screensize;

void main() {
	texidg = vec2(texid,highlight);
	uvstartg = uvstart;
	uvdimg = uvdim;
    gl_Position = vec4(position, -100.0, 1.0);
    //sizeg = vec2(size.x/screensize.x,size.y/screensize.y);
    sizeg = vec2(size.x,size.y);
	v_angle = angle;
	v_color = color;
}
