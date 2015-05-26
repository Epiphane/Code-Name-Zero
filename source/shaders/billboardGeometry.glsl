#version 330

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 gProjection;
uniform mat4 gView;
uniform vec3 gCameraPos;
uniform float gBillboardSize;

in float Type0[];

out vec2 TexCoord;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

void main()
{
   if (Type0[0] != PARTICLE_TYPE_LAUNCHER) {
      vec3 Pos = gl_in[0].gl_Position.xyz; // position of the particle in world space
      
      vec3 look = normalize(gCameraPos - Pos);
      vec3 right = normalize(cross(look, vec3(0.0, 1.0, 0.0)));
      vec3 up = cross(right, look);
      
      // Bottom left corner of the quad
      vec3 vertexA = Pos - (right + up) * gBillboardSize;
      gl_Position = gProjection * gView * vec4(vertexA, 1.0);
      TexCoord = vec2(0.0, 0.0);
      EmitVertex();
      
      // Upper left corner of the quad
      vec3 vertexB = Pos - (right - up) * gBillboardSize;
      gl_Position = gProjection * gView * vec4(vertexB, 1.0);
      TexCoord = vec2(0.0, 1.0);
      EmitVertex();
      
      // Bottom right corner of the quad
      vec3 vertexC = Pos + (right - up) * gBillboardSize;
      gl_Position = gProjection * gView * vec4(vertexC, 1.0);
      TexCoord = vec2(1.0, 0.0);
      EmitVertex();
      
      // Make the second triangle by creating a vertex at upper right corner
      vec3 vertexD = Pos + (right + up) * gBillboardSize;
      gl_Position = gProjection * gView * vec4(vertexD, 1.0);
      TexCoord = vec2(1.0, 1.0);
      EmitVertex();
      
      EndPrimitive();
   }
}
