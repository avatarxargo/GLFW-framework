#version 420 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location=0) in vec2 v_uv;

layout(location=2) uniform sampler2D fbotex1;
layout(location=3) uniform sampler2D fbotex2;
layout(location=4) uniform sampler2D fbotex3;
layout(location=5) uniform vec4 bgcol;
layout(location=6) uniform vec4 drawflags; //0 - faces, 1 - lines, 2 - normals

out vec4 fragColor;

void main() {
	bool line = (texture(fbotex2, v_uv).a > 0) && (drawflags.y > 0);
	vec3 col = texture(fbotex1, v_uv).rgb;
	if(line)
		col = texture(fbotex2, v_uv).rgb;
	//col += texture(fbotex3, v_uv).rgb;
	float visible = texture(fbotex1, v_uv).a * float(drawflags.x > 0);
	visible += texture(fbotex2, v_uv).a * float(drawflags.y > 0);
	//visible += texture(fbotex3, v_uv).a;
	visible = min(1,max(0,visible));
	fragColor = vec4(col*visible+bgcol.rgb*(1-visible), 1); //
	//fragColor = vec4(texture(fbotex1, v_uv).a,texture(2, v_uv).a,texture(fbotex3, v_uv).a,1);//vec4(bgcol.rgb*(1-visible),1); //
	return;
	//lineart
	float thickness = 2;
	float stepX = thickness/1280.0;
	float stepY = thickness/720.0;
	vec3 deltaW = texture(fbotex3, v_uv).xyz-texture(fbotex3, v_uv+vec2(-stepX,0)).xyz;
	vec3 deltaE = texture(fbotex3, v_uv).xyz-texture(fbotex3, v_uv+vec2(stepX,0)).xyz;
	vec3 deltaN = texture(fbotex3, v_uv).xyz-texture(fbotex3, v_uv+vec2(0,stepY)).xyz;
	vec3 deltaS = texture(fbotex3, v_uv).xyz-texture(fbotex3, v_uv+vec2(0,-stepY)).xyz;
	if(deltaW != vec3(0)|| deltaE !=  vec3(0) || deltaN != vec3(0)|| deltaS !=  vec3(0)) {
		fragColor = vec4(0,0,0,1);
	}
}
