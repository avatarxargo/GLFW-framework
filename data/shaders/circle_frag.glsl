#version 330 core

in vec2 g_texcoord;
in vec2 g_rad;
in vec4 g_color;
in vec4 g_params;

out vec4 fragColor;
out vec4 lineColor;
out vec4 normalColor;

void main() {
    float rad = length(g_texcoord);
	if(rad>=1)
		discard;
	if(rad <= 1-(g_rad.y/g_rad.x))
		discard;
    fragColor = g_color;
	if(g_params.x != 0) {
		fragColor = vec4(1,1,0,1); 
		normalColor = vec4(0,0,0,0);
	}
	if(g_params.y != 0) {
		fragColor = vec4(1,1,1,1); 
		normalColor = vec4(0,0,0,0);
	}
}
