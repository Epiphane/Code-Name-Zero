#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUV;

out vec2 UV;

void main() {
   // Output position of the vertex, in clip space
   // map [0..1024][0..768] to [-1..1][-1..1]
   vec2 vertexPosition = aPosition - vec2(1024 / 2, 768 / 2);
   vertexPosition /= vec2(1024 / 2, 768 / 2);
   gl_Position =  vec4(vertexPosition, 0, 1);

   // UV of the vertex. No special space for this one.
   UV = aUV;
}