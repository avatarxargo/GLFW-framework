#version 430

uniform vec2 groupcount;
uniform int count;
uniform mat4 N;

struct TrFaceInfo {
		vec4 normal;
		vec4 castnormal;
    float rating; //whether the face faces camera, for toon shading
    float pad1;
    float pad2;
    float pad3;
	};

layout(std430, binding = 0) buffer inDataA
{
    TrFaceInfo faceinfo_in[];//7 floats per face
};

/*layout(std430, binding = 2) buffer outData
{
    TrFaceInfo faceinfo_out[];//7 floats per face
};*/

layout (local_size_x = 16, local_size_y = 16) in;
void main() {
  //gl_GlobalInvocationID.xyz = gl_WorkGroupID * gl_WorkGroupSize + gl_LocalInvocationID
  //gl_LocalInvocationID.xyz
  //gl_WorkGroupID.xyz
  //gl_WorkGroupSize.xyz

  //convert 2D space into 1D space of faces

  uint linewidth = uint(groupcount.x)*gl_WorkGroupSize.x;
  uint myidx = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y*linewidth;
  if(myidx > count)
    return;

  //glm::vec4 projn = glm::normalize(camera.N*face->normal);
  vec4 projn = normalize(N*faceinfo_in[myidx].normal);
  //face->castnormal = projn;
  faceinfo_in[myidx].castnormal = projn;
  //face->rating = face->castnormal.z;
  faceinfo_in[myidx].rating = faceinfo_in[myidx].castnormal.z;
}
