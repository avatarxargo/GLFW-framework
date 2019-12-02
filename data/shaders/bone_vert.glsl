#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in vec4 pos1;
layout(location=1) in vec4 pos2;
layout(location=2) in vec4 highlight;
layout(location=3) in vec4 color;
layout(location=4) in vec4 transform;
// 5
// 6
// 7

layout(location=0) out vec4 v_pos2;
layout(location=1) out vec4 v_highlight;
layout(location=2) out vec4 v_color;
//layout(location=3) out mat4 v_transform;

layout(location=0) uniform mat4 mvp;
layout(location=1) uniform vec2 screensize;

void main() {
    gl_Position = pos1;
	v_pos2 = pos2;
    v_highlight = highlight;
	v_color = color;
	//v_transform = transform;
}
