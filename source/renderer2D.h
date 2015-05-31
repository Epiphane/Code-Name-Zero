//
//  renderer_2D.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/26/15.
//
//

#ifndef __RGBZero__renderer_2D__
#define __RGBZero__renderer_2D__

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "tiny_obj_loader.h"
#include "renderer.h"
#include "vertex_buffer_object.h"
#include "GLSL.h"

class Renderer2D : public Renderer {
private:
   VBO b_vertex, b_uv, b_opacity;
   GLuint elements, texture;
   
   float z;

   bool hasOpacity;
   
   static bool initialized;
   void init();
   static GLuint program;
   static GLuint uProj, uModel, uZValue;
   static GLuint uWinScale, uTexture, uHasOpacity;
   
public:
   Renderer2D(std::string texture) : Renderer2D(texture, false, 0) {};
   Renderer2D(std::string texture, float z) : Renderer2D(texture, false, z) {};
   Renderer2D(std::string texture, bool hasOpacity, float z);
   
   // Refers to the number of indices
   void setNumElements(GLuint num) { elements = num; }
   
   // Send data to the GPU
   void bufferData(DataType type, size_t size, void *data);
   void bufferData(DataType type, const std::vector<float>& data);
   void bufferData(DataType type, const std::vector<glm::vec2>& data);
   
   // Render the data!
   void render(glm::mat4 Model);
};

#endif /* defined(__RGBZero__renderer_2D__) */
