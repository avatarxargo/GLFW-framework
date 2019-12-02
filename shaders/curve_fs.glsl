#version 400

uniform sampler2D diffuse_tex;
uniform vec4 vecparam; //shade, animspeed, animsquish, animwobble
uniform float time;
uniform mat4 VM;

out vec4 fragcolor;
in vec2 tex_coord;
in vec3 position;
in vec3 pospos;
in vec3 normal;
in vec3 camnormal;

void main(void) {
   //fragcolor = vec4(1-position.z/5,position.z/5,position.z/5,1);
   //fragcolor = vec4(0,0,0,dot(camnormal,vec3(0,0,1)));
   fragcolor = vec4(0,0,0,1);
}
