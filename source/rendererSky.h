//
//  rendererSky.h
//  RGBZero
//
//  Created by Max Linsenbard on 5/25/15.
//
//

#ifndef __RGBZero__rendererSky__
#define __RGBZero__rendererSky__

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "tiny_obj_loader.h"
#include "renderer.h"
#include "GLSL.h"

class SkyRenderer : public Renderer {
private:
   GLuint v_buffer;
   GLuint elements, texture;
   
   float z;
   
   static bool initialized;
   void init();
   static GLuint program;
   static GLuint aPosition;
   static GLuint uInvProjMatrix, uInvViewMatrix, uLightDir, uViewport, uFramebufferResolution;
   
public:
   SkyRenderer(std::string texture, float z = 0);
   
   // Refers to the number of indices
   void setNumElements(GLuint num) { elements = num; }
   
   // Send data to the GPU
   void bufferData(DataType type, size_t size, void *data);
   void bufferData(DataType type, const std::vector<float>& data);
   void bufferData(DataType type, const std::vector<glm::vec2>& data);
   
   // Render the data!
   void render(glm::mat4 Model);
};


#endif /* defined(__RGBZero__rendererSky__) */
