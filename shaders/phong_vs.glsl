#version 400      
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;  
uniform vec4 myV;  
uniform mat3 N;
uniform vec4 L;  

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;
in vec4 param_attrib;

out vec2 tex_coord; 
out vec3 pospos; 
out vec3 normal; 
out vec4 param;

void main(void)
{
	gl_Position = P*VM*vec4(pos_attrib,1);
	pospos = (VM*vec4(pos_attrib,1)).xyz;
	normal = normalize(vec4(N*normal_attrib,0)).xyz;
	tex_coord = tex_coord_attrib;
}