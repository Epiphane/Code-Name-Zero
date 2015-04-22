#version 330 core

uniform sampler2D uTexUnit;

in vec2 UV;
out vec4 fragColor;

void main() {
   fragColor = texture( uTexUnit, UV );

   float d = abs(fragColor.r - fragColor.b);
   if (d > 0.6)
      discard;
   d = abs(fragColor.r - fragColor.g);
   if (d > 0.6)
      discard;

   // gl_FragColor = vec4(1, 1, 1, 1);
}