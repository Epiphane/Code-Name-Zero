#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aMaterial;
layout(location = 7) in mat4 aModelMatrix;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uCameraPos;
uniform vec3 uLightPos;

flat out int vMaterial;
out vec2 vUV;
out vec3 vNormal;
out vec4 vWorldSpace;
out vec3 vCameraVec;
out vec3 vLightPos;

#define M_PI 3.1415926535897932384626433832795

void main()
{
   vLightPos = uLightPos;
   vWorldSpace = aModelMatrix * aPosition;

   vCameraVec = normalize(uCameraPos - vWorldSpace.xyz);

   gl_Position = uProjMatrix * uViewMatrix * vWorldSpace;
   
   vMaterial = int(aMaterial);
   vUV = aUV;
   vNormal = normalize((aModelMatrix * vec4(aNormal, 0)).xyz);
}
