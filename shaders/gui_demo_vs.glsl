#version 400
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;

uniform vec4 vecparam; //shade, animspeed, animsquish, animwobble
uniform float time;

layout(location=0) in vec3 pos_attrib;
layout(location=1) in vec3 normal_attrib;
layout(location=2) in vec2 tex_coord_attrib;
layout(location=3) in vec4 color_attrib;

out vec2 tex_coord;
out vec3 pospos;
out vec3 normal;
out vec4 color;

void main(void)
{
   float animspeed = vecparam[1];
   float animsquish = vecparam[2];
   float animwobble = vecparam[3];
   gl_Position = PVM*vec4(pos_attrib, 1.0);
   pospos = gl_Position.xyz;
   normal = normal_attrib;
   tex_coord = tex_coord_attrib;
   color = color_attrib;
}
