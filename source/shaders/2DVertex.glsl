#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aUV;

uniform mat4 uProjMatrix;
uniform mat4 uModelMatrix;

out vec2 vUV;

void main()
{
   gl_Position = uProjMatrix * uModelMatrix * vec4(aPosition, 1);
   
   vUV = aUV;
}
