#version 420 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location=0) in vec2 v_uv;

layout(location=2) uniform sampler2D fbotex1;
layout(location=3) uniform sampler2D fbotex2;
layout(location=4) uniform sampler2D fbotex3;

out vec4 fragColor;

void main() {
	fragColor = vec4(texture(fbotex1, v_uv).xyz, 0) + texture(fbotex2, v_uv);
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
