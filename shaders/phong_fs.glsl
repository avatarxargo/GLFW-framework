#version 400

uniform sampler2D diffuse_tex;
uniform float time;

// light (view space)
uniform vec3 lpos; //in camera coordinates
uniform vec3 lamb;
uniform vec3 ldiff;
uniform vec3 lspec;
uniform vec3 lfalloff;

// material
uniform vec3 mamb;
uniform vec3 mdiff;
uniform vec3 mspec;
uniform int mshininess;
uniform bool mtextured;
         
in vec2 tex_coord;
in vec3 pospos; //(view space)
in vec3 normal; //(view space)
in vec4 param;

out vec4 fragcolor;  

void main(void)
{   

	// ================ LIGHT ===================

	vec3 L = normalize(lpos-pospos);
	float langle = dot(L,normal);
	float ldiffint = max(langle,0);
	float ldist = distance(lpos,pospos);

	// Specular
	vec3 V = vec3(0,0,1);
	//vec3 h = 0.5*(L+V);
	//float lspecint = pow(max(dot(h,normal),0),mshininess);
	float angle = dot(normal,L); //dot as cos.
	vec3 r = L+2*(normal*angle-L);
	float lspecint = pow(max(dot(V,r),0),mshininess);

	// ================ RENDER ===================

	vec4 tex_color = texture(diffuse_tex, tex_coord);
	if(!mtextured) {
		tex_color = vec4(1.0);
	}
	vec3 pt_amb = mamb*lamb;
	vec3 pt_diff = mdiff*ldiffint*ldiff;
	vec3 pt_spec = mspec*lspecint*lspec;
	
	float qAttenuation = 1+lfalloff.x+lfalloff.y*ldist+lfalloff.z*ldist*ldist;
	vec3 amb_component = pt_amb*tex_color.xyz;
	vec3 diff_component = (1.0/vec3(qAttenuation))*pt_diff*tex_color.xyz;
	vec3 spec_component = pt_spec;
	vec3 I =  min(amb_component + diff_component + spec_component,1.0);
	fragcolor = vec4(I,tex_color.a);
	//fragcolor = vec4(normalize(normal)*0.5+0.5,1.0)*vec4(pt_amb+pt_diff,1.0);
}