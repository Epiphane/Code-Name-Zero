#version 330 core

layout (lines) in;
layout (line_strip, max_vertices = 64) out;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

in int shape[];
out vec3 fColor;

void emit(vec4 vert) {
   gl_Position = uProjMatrix * uViewMatrix * vert;
   EmitVertex();
}

#define M_PI 3.1415926535897932384626433832795
#define INC_DEG M_PI / 8

void main()
{
   if (shape[0] == 0) { // Circle
      fColor = vec3(1, 1, 0);
      
      float r = gl_in[1].gl_Position.x;
      vec4 center = gl_in[0].gl_Position;
      
      // Z = 0
      for (float degrees = 0.0; degrees <= 2 * M_PI; degrees += INC_DEG) {
         vec4 vert = center + vec4(r * sin(degrees), r * cos(degrees), 0, 0);
         emit(vert);
      }
      // X = 0
      for (float degrees = M_PI / 2; degrees <= 2 * M_PI; degrees += INC_DEG) {
         vec4 vert = center + vec4(0, r * sin(degrees), r * cos(degrees), 0);
         emit(vert);
         // Don't complete this circle, do the horizontal one first
      }
      // Y = 0
      for (float degrees = 0.0; degrees <= 2 * M_PI; degrees += INC_DEG) {
         vec4 vert = center + vec4(r * sin(degrees), 0, r * cos(degrees), 0);
         emit(vert);
      }
      // Finish X = 0
      for (float degrees = 0.0; degrees <= M_PI / 2; degrees += INC_DEG) {
         vec4 vert = center + vec4(0, r * sin(degrees), r * cos(degrees), 0);
         emit(vert);
      }
   }
   else if (shape[0] == 1) { // Bounds
      fColor = vec3(1, 0, 0);
      
      mat4 compMatrix = uProjMatrix * uViewMatrix;
      vec4 min = gl_in[0].gl_Position;
      vec4 max = gl_in[1].gl_Position;
      
      // Back face
      vec4 vert = min; emit(vert);
      vert.x = max.x;  emit(vert);
      vert.y = max.y;  emit(vert);
      vert.x = min.x;  emit(vert);
      vert.y = min.y;  emit(vert);
      
      // Front face
      vert.z = max.z;  emit(vert);
      vert.x = max.x;  emit(vert);
      vert.y = max.y;  emit(vert);
      vert.x = min.x;  emit(vert);
      vert.y = min.y;  emit(vert);
      
      // Snake around the edges
      vert.x = max.x;  emit(vert);
      vert.z = min.z;  emit(vert);
      vert.y = max.y;  emit(vert);
      vert.z = max.z;  emit(vert);
      vert.x = min.x;  emit(vert);
      vert.z = min.z;  emit(vert);
   }
   else {
      fColor = vec3(1, 1, 1);
   }
   
   EndPrimitive();
}