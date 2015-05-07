//
//  renderer_3D.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/26/15.
//
//

#ifndef __RGBZero__renderer_3D__
#define __RGBZero__renderer_3D__

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "vertex_buffer_object.h"
#include "tiny_obj_loader.h"
#include "renderer.h"
#include "GLSL.h"

#define MAX_MATERIALS 50

class Renderer3D : public Renderer {
private:
   const static int NUM_BUFFERS = 5;
   // Indices at which we keep each buffer
   const static int b_vertex   = 0;
   const static int b_uv       = 1;
   const static int b_normal   = 2;
   const static int b_index    = 3;
   const static int b_material = 4;
   
   GLuint elements;
   VBO *buffers[NUM_BUFFERS];
   
   GLuint numMaterials;
   glm::vec3 ambient[MAX_MATERIALS], diffuse[MAX_MATERIALS];
   glm::vec3 specular[MAX_MATERIALS];
   float shine[MAX_MATERIALS];
   glm::vec2 textureScale[MAX_MATERIALS];
   GLuint texture;
   
   bool hasTextures;
   
   static bool initialized;
   void init();
   static GLuint program;
   static GLuint uProj, uModel, uView;
   static GLuint uWinScale, uLightPos, uCameraPos;
   static GLuint uAColor, uDColor;
   static GLuint uSColor, uShine;
   static GLuint aPosition, aNormal;
   static GLuint uTexScale, uTexUnits, uHasTextures;
   
public:
   Renderer3D();
   
   // Refers to the number of indices
   void setNumElements(GLuint num) { elements = num; }
   
   // Send data to the GPU
   void bufferData(DataType type, size_t size, void *data);
   void bufferData(DataType type, const std::vector<float>& data);
   void bufferData(DataType type, const std::vector<glm::vec3>& data);
   // SPECIAL CASE: DataType `indices` also sets numElements!!
   void bufferData(DataType type, const std::vector<int>& data);
   void bufferData(DataType type, const std::vector<unsigned int>& data);
   
   // Set materials for an object
   void setMaterials(std::string baseDir, const std::vector<tinyobj::material_t>& data);
   
   // Render the data!
   void render(glm::mat4 Model);
   
   Renderer *clone();
};

#endif /* defined(__RGBZero__renderer_3D__) */
