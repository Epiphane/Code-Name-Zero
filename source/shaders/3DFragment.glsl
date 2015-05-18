#version 330 core

flat in int vMaterial;
in vec2 vUV;
in vec3 vNormal;
in vec4 vWorldSpace;

in vec3 vLightPos;
in vec3 vCameraVec;

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

uniform vec3 uShipTint;

out vec4 fragColor;
in vec4 vShadowCoord;

uniform sampler2DShadow uShadowMap;

void main() {
   vec3 aColor = vec3(0);
   vec3 dColor = vec3(0);
   vec3 sColor = vec3(0);
   float shine = 0;
   vec3 lightVector = normalize(vLightPos - vWorldSpace.xyz);
   vec3 cameraVec = normalize(vCameraVec);
   
   float Id = max(dot(vNormal, lightVector), 0.0f);
   
   // Calculate a visibility value
   float bias = 0.005 * tan(acos(Id));
   bias = clamp(bias, 0.0, 0.01);
   float visibility = texture(uShadowMap, vec3(vShadowCoord.xy, vShadowCoord.z - bias));
   
   
   
   if (vMaterial < 0) {
      // Placeholder: Basic metal
      aColor = vec3(0.15, 0.15, 0.15);
      dColor = vec3(0.4, 0.4, 0.4);
      sColor = vec3(0.14, 0.14, 0.14);
      shine = 76.8;
	  
	  float Is = pow(max(dot(vNormal, normalize(lightVector + cameraVec)), 0.0f), shine);
	  
	  fragColor = vec4(Is * sColor * visibility + Id * dColor * (visibility+0.2) + (aColor*1.2), 1);
     fragColor += vec4(uShipTint, 1.0);
//	  fragColor = vec4(Id * dColor + aColor, 1);
   }
   else {
      aColor = UaColor[vMaterial];
      dColor = UdColor[vMaterial];
      sColor = UsColor[vMaterial];
      shine = Ushine[vMaterial];
      
      if (uHasTextures) {
         vec2 UV = vec2(vUV.x * uTexScale[vMaterial].x,
                     vUV.y * uTexScale[vMaterial].y);
//         fragColor = texture(uTexUnits, vec3(UV, vMaterial));
		vec3 textureColor = texture(uTexUnits, vec3(UV, vMaterial)).xyz;

         //float Id = max(dot(vNormal, lightVector), 0.0f);
         float Is = pow(max(dot(vNormal, normalize(lightVector + cameraVec)), 0.0f), 50);
         
         fragColor = vec4(Is * vec3(1)  * visibility + Id * textureColor * (visibility+0.2) + (textureColor*1.2), 1);
         fragColor += vec4(uShipTint, 1.0);
      }
      else {
         //float Id = max(dot(vNormal, lightVector), 0.0f);
         float Is = pow(max(dot(vNormal, normalize(lightVector + cameraVec)), 0.0), shine);
         
         fragColor = vec4(Is * sColor * visibility + Id * dColor * (visibility+0.2) + (aColor*1.2), 1);
         fragColor += vec4(uShipTint, 1.0);
//         fragColor = vec4(Id * dColor + aColor, 1);
      }
      
      if (fragColor.a == 0)
         discard;
   }
}
