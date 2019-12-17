#version 400

uniform sampler2D diffuse_tex;
uniform vec4 vecparam; //shade, animspeed, animsquish, animwobble
uniform float time;
uniform mat4 VM;

out vec4 fragcolor;
out vec4 lineColor;
out vec4 normalColor;

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
in vec3 pospos;
in vec3 normal;
in vec3 camnormal;
in vec4 color;

void main(void) {
   //fragcolor = vec4(normal*0.5+0.5,1);
   fragcolor = vec4(camnormal*0.5+0.5,1);
   const int segments = 20;
   const int ratio = 20;
   //fragcolor*=1+2*float((int(tex_coord.x*segments*ratio)%segments==1 || int(tex_coord.y*segments*ratio)%segments==1 ));
   //fragcolor = vec4(normal*0.5+0.5,1);
    //fragcolor = vec4(0.3,0.3,0.3,1);
    //fragcolor.a = 1;

  	fragcolor = vec4(normalize(camnormal)*0.5+0.5,1.0);
    if(normalize(camnormal).z>0) {
      fragcolor = vec4(0,1,0,1);
    } else {
        fragcolor = vec4(1,0,0,1);
    }

    //////
    //////
    /////
    vec3 L = normalize(lpos-pospos);
  	float langle = dot(L,camnormal);
  	float ldiffint = langle;//max(langle,0);
    float shadediff = ldiffint-0.5;
    bool shaded = ldiffint <= 0;
    if(!shaded) {
      ldiffint = 1;
    } else {
      ldiffint = 0.5;
    }
  	float ldist = distance(lpos,pospos);

  	// Specular
  	vec3 V = vec3(0,0,1);
  	//vec3 h = 0.5*(L+V);
  	//float lspecint = pow(max(dot(h,normal),0),mshininess);
  	float angle = dot(camnormal,L); //dot as cos.
  	vec3 r = L+2*(camnormal*angle-L);
  	float lspecint = pow(max(dot(V,r),0),mshininess);

  	// ================ RENDER ===================

  	vec4 tex_color = texture(diffuse_tex, tex_coord);
    vec4 shadow_color;
  	//if(!mtextured) {
  		shadow_color = vec4(normalize(camnormal)*0.5+0.5,1.0);
    	shadow_color = vec4(1.0);

      float alt = 1-abs(shadediff);
      float nor = abs(shadediff);
      float blend = 1;
      if(nor>0.4) {
        float ratio = 10*(0.5-nor);
        blend = ratio;
      }
      if(shaded) {
        shadow_color = vec4(0.3+nor*0.2,0.2+nor*0.3,0.2+0.4*nor,1);
      } else {
        shadow_color = vec4(vec3(0.4*blend+0.6),1);
      }

      if(!mtextured) {
        shadow_color = vec4(1.0);
      }
  //	}
  	vec3 pt_amb = mamb*lamb;
  	vec3 pt_diff = mdiff*ldiffint*ldiff;
  	vec3 pt_spec = mspec*lspecint*lspec;

  	float qAttenuation = 1+lfalloff.x+lfalloff.y*ldist+lfalloff.z*ldist*ldist;
  	vec3 amb_component = pt_amb*tex_color.xyz;
  	vec3 diff_component = shadow_color.xyz*tex_color.xyz;
  	vec3 spec_component = pt_spec;
  	vec3 I =  min(amb_component+diff_component+spec_component,1.0);
  	fragcolor = vec4(I,tex_color.a);
    lineColor = vec4(0,0,0,0);
    normalColor = vec4(camnormal,1);
 }
