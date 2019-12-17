#version 400
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;
uniform mat4 N;

uniform vec4 vecparam; //shade, animspeed, animsquish, animwobble
uniform float time;

layout(location=0) in vec3 pos_attrib;
layout(location=1) in vec3 normal_attrib;
layout(location=2) in vec2 tex_coord_attrib;
layout(location=3) in vec4 color_attrib;

out vec2 tex_coord;
out vec3 position;
out vec3 pospos;
out vec3 normal;
out vec3 camnormal;

void main(void)
{
   float animspeed = vecparam[1];
   float animsquish = vecparam[2];
   float animwobble = vecparam[3];
   gl_Position = VM*vec4(pos_attrib, 1.0);
   position = gl_Position.xyz;
   pospos = gl_Position.xyz;
   normal = normal_attrib;
   camnormal = normalize(N*vec4(normal_attrib,0)).xyz;
   tex_coord = tex_coord_attrib;
}
