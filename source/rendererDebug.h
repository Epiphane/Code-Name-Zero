//
//  renderer_2D.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/26/15.
//
//

#ifndef __RGBZero__renderer_Debug__
#define __RGBZero__renderer_Debug__

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "tiny_obj_loader.h"
#include "renderer.h"
#include "GLSL.h"
#include "bounds.h"

const int SHAPE_CIRCLE = 0;
const int SHAPE_BOX = 1;

class RendererDebug : public Renderer {
private:
   const static int NUM_BUFFERS = 2;
   // Indices at which we keep each buffer
   const static int b_vertex   = 0;
   const static int b_shape    = 1;
   
   static bool initialized;
   void init();
   static GLuint program;
   static GLuint uProj, uView, uWinScale;
   static GLuint aShape, aPosition;
   
   GLuint buffers[NUM_BUFFERS];
   
   std::vector<glm::vec3> positions;
   std::vector<int> shapes;
   
   RendererDebug();
public:
   static RendererDebug *instance() {
      static RendererDebug *inst = new RendererDebug();
      return inst;
   }
   
   void renderCircle(glm::vec3 center, float radius);
   void renderBounds(glm::vec3 center, const Bounds &bounds);
   
   // Render the data!
   void render();
};

#endif /* defined(__RGBZero__renderer_Debug__) */
