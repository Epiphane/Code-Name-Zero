#version 330 core

uniform mat depthMVP;

void main() {
   gl_Position = depthMVP * vec4(vertexPosition_modelspace,1.0);
}