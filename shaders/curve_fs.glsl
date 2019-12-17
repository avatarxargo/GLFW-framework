#version 400

uniform sampler2D diffuse_tex;
uniform sampler2D stroke_diffuse_tex;
uniform vec4 vecparam; //shade, animspeed, animsquish, animwobble
uniform float time;
uniform mat4 VM;

out vec4 fragcolor;
out vec4 lineColor;
out vec4 normalColor;

in vec2 tex_coord_g;

void main(void) {
   //fragcolor = vec4(1-position.z/5,position.z/5,position.z/5,1);
   //fragcolor = vec4(0,0,0,dot(camnormal,vec3(0,0,1)));
   fragcolor = vec4(0,0,0,0);
   lineColor = texture(stroke_diffuse_tex, tex_coord_g);
   normalColor = vec4(0,0,0,0);
}
