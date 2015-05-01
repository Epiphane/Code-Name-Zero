#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in int aShape;

out int shape;

void main()
{
   gl_Position = vec4(aPosition, 1);
   
   shape = aShape;
}
