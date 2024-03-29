#version 330 core

// Interpolated values from the geometry shaders
in vec2 fUV;
in float fOpacity;

// Ouput data
out vec4 fragColor;

uniform sampler2D uTexture;

void main()
{
   //fragColor = vec4(fUV.x, fUV.y, 1, 1);
   fragColor = texture(uTexture, fUV);
   fragColor.a *= fOpacity;

   if (fragColor.a == 0) discard;
}