#version 330 core

in vec4 g_highlight;s
in vec4 g_color;

out vec4 fragColor;
out vec4 lineartColor;

void main() {
	fragColor = vec4(0,0,0,0);
	lineartColor = vec4(1,0,0,1); 
}
