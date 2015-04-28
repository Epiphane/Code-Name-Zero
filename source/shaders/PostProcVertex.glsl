#version 330 core

in vec2 v_coord;
uniform sampler2D fbo_texture;
out vec2 vTexcoord;
out vec2 vOffset;

void main(void) {
  gl_Position = vec4(v_coord, 0.0, 1.0);
  vTexcoord = (v_coord + 1.0) / 2.0;
  vOffset = v_coord;
}