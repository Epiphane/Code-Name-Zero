#version 330

layout (location = 0) in float Type;
layout (location = 1) in vec3 Position;

out float Type0;

void main()
{
   gl_Position = vec4(Position, 1.0);
   Type0 = Type;
}