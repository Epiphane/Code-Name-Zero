#version 330

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 gProjection;
uniform mat4 gView;
uniform vec3 gCameraPos;
uniform float gBillboardSize;

in float Type0[];
in float Age0[];

out vec2 TexCoord;
out float ParticleAge;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f
#define ICON_SIZE 8.0f
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define SHELL_AGE 900.0f

void main()
{
   if (Type0[0] != PARTICLE_TYPE_LAUNCHER) {
      ParticleAge = Age0[0];
      vec3 Pos = gl_in[0].gl_Position.xyz; // position of the particle in world space
      
      vec4 clip = gProjection * gView * vec4(Pos, 1.0);
      float invWidth = 1.0/WINDOW_WIDTH;
      float invHeight = 1.0/WINDOW_HEIGHT;
      
      // Make particles bigger as they are about to die
      float iconSize = ICON_SIZE + (10 * ICON_SIZE * Age0[0] / SHELL_AGE);
      
      // Used to unproject the XY plane
      float dx = iconSize * invWidth * 0.5 * clip.w;
      float dy = iconSize * invHeight * 0.5 * clip.w;
      
      vec3 temp = gBillboardSize*gCameraPos; // not using gCameraPos

      // Bottom left corner of the quad
      gl_Position = vec4(clip.x - dx, clip.y + dy, clip.z, clip.w);
      TexCoord = vec2(0.0, 0.0);
      EmitVertex();
      
      // Upper left corner of the quad
      gl_Position = vec4(clip.x - dx, clip.y - dy, clip.z, clip.w);
      TexCoord = vec2(0.0, 1.0);
      EmitVertex();
      
      // Bottom right corner of the quad
      gl_Position = vec4(clip.x + dx, clip.y + dy, clip.z, clip.w);
      TexCoord = vec2(1.0, 0.0);
      EmitVertex();
      
      // Make the second triangle by creating a vertex at upper right corner
      gl_Position = vec4(clip.x + dx, clip.y - dy, clip.z, clip.w);
      TexCoord = vec2(1.0, 1.0);
      EmitVertex();
      
      EndPrimitive();
   }
}

