#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in vec4 posrad; //posx, posy, rad1, rad2
layout(location=1) in vec4 params;
layout(location=2) in vec4 color;

layout(location=0) out vec2 v_rad;
layout(location=1) out vec4 v_params;
layout(location=2) out vec4 v_color;

layout(location=0) uniform mat4 mvp;
layout(location=1) uniform vec2 screensize;

void main() {
    gl_Position = mvp * vec4(posrad.xy, 0.0, 1.0);
    v_rad = posrad.zw;
	v_params = params;
	v_color = color;
}
