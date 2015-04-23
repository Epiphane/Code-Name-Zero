#version 330 core

uniform sampler2D fbo_texture;
in vec2 vTexcoord;

out vec4 fragColor;
 
void main(void) {
  fragColor = texture2D(fbo_texture, vTexcoord) + vec4(0.4f, 0, 0, 0);
}