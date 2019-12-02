#version 330 core

in vec2 textureCoord;
in vec2 texid;
in vec4 g_color;

out vec4 fragColor;
out vec4 lineartColor;
out vec4 normalColor;

uniform sampler2D texImage1;
uniform sampler2D texImage2;
uniform sampler2D texImage3;
uniform sampler2D texImage4;

void main() {
	bool highlight = (int(texid[1]) != 0);
	/*if(highlight && (textureCoord.x<0.05 || textureCoord.x>0.95 || textureCoord.y<0.05 || textureCoord.y>0.95)) {
		//fragColor = vec4(0,0,0,0);
		fragColor = vec4(1,1,0,1);
		return;
	}*/
	vec4 textureColor;
	if(int(texid[0]) == 0) {
		textureColor = texture(texImage1, textureCoord);
	} else if(int(texid[0]) == 1) {
		textureColor = texture(texImage2, textureCoord);
	} else if(int(texid[0]) == 2) {
		textureColor = texture(texImage3, textureCoord);
	} else if(int(texid[0]) == 3) {
		textureColor = texture(texImage4, textureCoord);
	}
	if(highlight) {
		textureColor *= vec4(2,2,2,1);
		if(textureColor.a < 0.2) {
			textureColor = vec4(1,0.5,1,0.2);
		}
	}
    fragColor = textureColor;
	lineartColor = vec4(0,0,0,1);
	if(textureColor.a > 0.2) {
		normalColor = g_color;
	} else {
		normalColor = vec4(0,0,0,0);
	}
}
