#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aMaterial;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uCameraPos;
uniform vec3 uLightPos;
uniform vec3 uShipPos;

// Shadow pass things
uniform mat4 uShadowView;
uniform mat4 uShadowProj;
// Ship tint
uniform vec3 uShipTint;

flat out int vMaterial;
out vec2 vUV;
out vec3 vNormal;
out vec4 vWorldSpace;
out vec3 vCameraVec;
out vec3 vLightPos;
// Shadow pass things
out vec4 vShadowCoord;

#define M_PI 3.1415926535897932384626433832795

void main()
{
   vLightPos = uLightPos;
   vWorldSpace = uModelMatrix * aPosition;

   vCameraVec = normalize(uCameraPos - vWorldSpace.xyz);

   gl_Position = uProjMatrix * uViewMatrix * vWorldSpace;

   /*
   gl_Position = uViewMatrix * vWorldSpace;

   mat4 ProjMat2 = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
   vec4(0.0f, 1.0f, 0.0f, 0.0f),
   vec4(0.0f, 0.01f * (gl_Position.z) * (gl_Position.z), 1.0f, 0.0f),
   vec4(0.0f, 0.0f, 0.0f, 0.0f));

   gl_Position = uProjMatrix * ProjMat2 * gl_Position;
   */


   float offset = -1.0f * (vWorldSpace.z - 0);// - uShipPos.z); // Offset z by current ship position
   gl_Position.y += 0.0009f * offset * offset;

   vMaterial = int(aMaterial);
   vUV = aUV;
   vNormal = normalize((uModelMatrix * vec4(aNormal, 0)).xyz);

   // and now Ryan makes up normals...
   // This makes normals behave with the upward curve of the track
   vNormal.z += 0.000001f * offset * offset;
   vNormal = normalize(vNormal);

   vShadowCoord = uShadowProj * uShadowView * uModelMatrix * aPosition;
}