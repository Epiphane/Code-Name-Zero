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
#include "GLSL.h"

class Renderer2D : public Renderer {
private:
   const static int NUM_BUFFERS = 2;
   // Indices at which we keep each buffer
   const static int b_vertex   = 0;
   const static int b_uv       = 1;
   
   GLuint buffers[NUM_BUFFERS];
   GLuint elements, texture;
   
   float z;
   
   static bool initialized;
   void init();
   static GLuint program;
   static GLuint uProj, uModel, uZValue;
   static GLuint uWinScale, uTexture;
   static GLuint aPosition, aUV;
   
public:
   Renderer2D(std::string texture, float z = 0);
   
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
