#version 330

layout (location = 0) in float Type;
layout (location = 1) in vec3 Position;
layout (location = 2) in float Age;

out float Type0;
out float Age0;

void main()
{
   gl_Position = vec4(Position, 1.0);
   Type0 = Type;
   Age0 = Age;
}