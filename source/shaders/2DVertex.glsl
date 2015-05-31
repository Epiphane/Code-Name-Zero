#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float opacity;

uniform mat4 uProjMatrix;
uniform mat4 uModelMatrix;
uniform bool hasOpacity;

out vec2 vUV;
out float vOpacity;

void main()
{
   gl_Position = uProjMatrix * uModelMatrix * vec4(aPosition, 1);
   
   if (hasOpacity)
      vOpacity = opacity;
   else
      vOpacity = 1;
   vUV = aUV;
}
