#version 330 core

in vec2 vUV;
in vec3 vNormal;
in vec4 vWorldSpace;

uniform vec3 uLightPos;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;

uniform sampler2D uTexUnit;

uniform int uShadeModel;
uniform int uShowNormal;

out vec4 fragColor;

void main() {
   vec3 lightVector = normalize(uLightPos - vWorldSpace.xyz);
   float u = vUV.x;
     
   float Id = dot(normalize(vNormal), lightVector);
   float Is = pow(dot(normalize(vNormal), normalize(lightVector + vWorldSpace.xyz)), Ushine);

   fragColor = vec4(Is * UsColor + Id * UdColor + UaColor, 1);
}
