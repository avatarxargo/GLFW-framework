#version 420 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in vec4 pos; //posx, posy, rad1, rad2
layout(location=1) in vec4 uv;

layout(location=0) out vec2 v_uv;

layout(location=0) uniform mat4 mvp;
layout(location=1) uniform vec2 screensize;

void main() {
    gl_Position = mvp * pos;
    v_uv = uv.xy;
}
