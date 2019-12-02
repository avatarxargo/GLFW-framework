#version 420 core
#extension GL_ARB_explicit_attrib_location : enable

layout(location=1) in vec4 g_highlight;
layout(location=2) in vec4 g_color;

out vec4 fragColor;
out vec4 lineartColor;

void main() {
	fragColor = vec4(0,0,0,0); 
	lineartColor = vec4(1,0,0,1); 
}
