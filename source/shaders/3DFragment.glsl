#version 330 core

flat in int vMaterial;
in vec2 vUV;
in vec3 vNormal;
in vec4 vWorldSpace;

uniform vec3 uLightPos;

#define MAX_MATERIALS 30

uniform vec3 UaColor[MAX_MATERIALS];
uniform vec3 UdColor[MAX_MATERIALS];
uniform vec3 UsColor[MAX_MATERIALS];
uniform float Ushine[MAX_MATERIALS];

uniform bool uHasTextures;
uniform vec2 uTexScale[MAX_MATERIALS];
uniform sampler2DArray uTexUnits;

uniform int uShadeModel;
uniform int uShowNormal;

out vec4 fragColor;

void main() {
   vec3 aColor = vec3(0);
   vec3 dColor = vec3(0);
   vec3 sColor = vec3(0);
   float shine = 0;
   vec3 lightVector = normalize(uLightPos - vWorldSpace.xyz);
   
   if (vMaterial < 0) {
      // Placeholder: Basic metal
      aColor = vec3(0.15, 0.15, 0.15);
      dColor = vec3(0.4, 0.4, 0.4);
      sColor = vec3(0.14, 0.14, 0.14);
      shine = 76.8;
	  
	  float Id = dot(normalize(vNormal), lightVector);
	  float Is = pow(dot(normalize(vNormal), normalize(lightVector + vWorldSpace.xyz)), shine);
	  
	  fragColor = vec4(Is * sColor + Id * dColor + aColor, 1);
	  fragColor = vec4(Id * dColor + aColor, 1);
   }
   else {
      aColor = UaColor[vMaterial];
      dColor = UdColor[vMaterial];
      sColor = UsColor[vMaterial];
      shine = Ushine[vMaterial];
      
      if (uHasTextures) {
         vec2 UV = vec2(vUV.x * uTexScale[vMaterial].x,
                     vUV.y * uTexScale[vMaterial].y);
         fragColor = texture(uTexUnits, vec3(UV, vMaterial));
      }
      else {
         float Id = dot(normalize(vNormal), lightVector);
         float Is = pow(dot(normalize(vNormal), normalize(lightVector + vWorldSpace.xyz)), shine);
         
         fragColor = vec4(Is * sColor + Id * dColor + aColor, 1);
         fragColor = vec4(Id * dColor + aColor, 1);
      }
      
      if (fragColor.a == 0)
         discard;
   }
}
