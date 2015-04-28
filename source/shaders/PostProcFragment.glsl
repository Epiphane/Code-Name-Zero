#version 330 core

uniform sampler2D fbo_texture;
uniform int blur;

in vec2 vOffset;
in vec2 vTexcoord;

out vec4 fragColor;
//#define blur 40f
 
void main(void) {
  fragColor = texture(fbo_texture, vTexcoord);

  if (blur > 0) {
     for (int i = 0; i < blur; i++) {
        fragColor += texture(fbo_texture, vTexcoord + (vOffset / 1000f * i));
     }
     fragColor /= blur;
  }
}