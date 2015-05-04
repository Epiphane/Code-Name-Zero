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
#include <queue>
#include <glm/glm.hpp>

#include "tiny_obj_loader.h"
#include "renderer.h"
#include "renderer2D.h"
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
   
   GLuint buffers[NUM_BUFFERS];
   
   std::vector<glm::vec3> positions;
   std::vector<int> shapes;
   std::queue<std::string> transient_log;
   std::vector<std::string> persistent_log;
   
   Renderer2D *log_renderer;
   
   RendererDebug();
public:
   static RendererDebug *instance();
   
   void renderCircle(glm::vec3 center, float radius);
   void renderBounds(glm::vec3 center, const Bounds &bounds);
   
   // Write text to the debug log
   void log(std::string text, bool persistent);
   
   // Render the data!
   void clearLog() { transient_log.empty(); persistent_log.clear(); }
   void renderLog();
   void render(glm::mat4 model);
};

#endif /* defined(__RGBZero__renderer_Debug__) */
