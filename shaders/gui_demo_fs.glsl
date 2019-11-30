#version 400

uniform sampler2D diffuse_tex;
uniform vec4 vecparam; //shade, animspeed, animsquish, animwobble
uniform float time;
uniform mat4 VM;

out vec4 fragcolor;
in vec2 tex_coord;
in vec3 pospos;
in vec3 normal;

void main(void) {
   fragcolor = vec4(normal*0.5+0.5,1);
   const int segments = 20;
   const int ratio = 20;
   fragcolor*=1+2*float((int(tex_coord.x*segments*ratio)%segments==1 || int(tex_coord.y*segments*ratio)%segments==1 ));
 }
