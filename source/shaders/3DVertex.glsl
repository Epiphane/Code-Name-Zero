#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aMaterial;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform vec3 uLightPos;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform int uShadeModel;
uniform int uShowNormal;

out vec2 vUV;
out vec3 vNormal;
out vec4 vWorldSpace;

#define M_PI 3.1415926535897932384626433832795

void main()
{
   vWorldSpace = uViewMatrix * uModelMatrix * aPosition;
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPosition;
        
   vUV = aUV;
   vNormal = (uViewMatrix * uModelMatrix * vec4(aNormal, 0)).xyz;
}
