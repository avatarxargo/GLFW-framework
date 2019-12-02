#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in vec4 boundingBox;
layout(location=1) in vec4 guide1;
layout(location=2) in vec4 guide2;
layout(location=3) in vec4 highlight;
layout(location=4) in vec4 color;

layout(location=0) out vec4 v_guides;
layout(location=1) out vec4 v_highlight;
layout(location=2) out vec4 v_color;

layout(location=0) uniform mat4 mvp;
layout(location=1) uniform vec2 screensize;

void main() {
    gl_Position = mvp * boundingBox;
    v_guides = vec4((mvp *guide1).xy,(mvp *guide2).xy);
	v_highlight = highlight;
	v_color = color;
}
