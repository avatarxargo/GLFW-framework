#version 400
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;

uniform vec4 vecparam; //shade, animspeed, animsquish, animwobble
uniform float time;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 tex_coord;
out vec3 pospos;
out vec3 normal; 

void main(void)
{
   float animspeed = vecparam[1];
   float animsquish = vecparam[2];
   float animwobble = vecparam[3];
   gl_Position = PVM*vec4(pos_attrib, 1.0);
   pospos = gl_Position.xyz;
   normal = normal_attrib;
   tex_coord = tex_coord_attrib;
}
