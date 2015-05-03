//
//  renderer_3D.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/26/15.
//
//

#include "main.h"
#include "shader.hpp"
#include "camera.h"
#include "texture.h"
#include "renderer.h"
#include "rendererDebug.h"

GLuint RendererDebug::program, RendererDebug::uWinScale, RendererDebug::uProj;
GLuint RendererDebug::uView;

bool RendererDebug::initialized = false;
void RendererDebug::init() {
   program = LoadShaders("./shaders/DebugVertex.glsl", "./shaders/DebugGeometry.glsl", "./shaders/DebugFragment.glsl");
   uWinScale = glGetUniformLocation(program, "windowScale");
   uProj = glGetUniformLocation(program, "uProjMatrix");
   uView = glGetUniformLocation(program, "uViewMatrix");
   
   GLenum error = glGetError();
   assert(error == 0);
   
   initialized = true;
}

RendererDebug::RendererDebug() : Renderer(0) {
   if (!initialized)
      init();
   
   positions.clear();
   shapes.clear();
   debug_log.clear();
   
   log_renderer = new Renderer2D("./textures/font.png");
   
   glGenBuffers(NUM_BUFFERS, buffers);
   
   GLenum error = glGetError();
   assert(error == 0);
};

void RendererDebug::renderCircle(glm::vec3 center, float radius) {
   positions.push_back(center); // First point: Center of circle
   positions.push_back(glm::vec3(radius)); // Second "point": radius
   shapes.push_back(SHAPE_CIRCLE);
   shapes.push_back(SHAPE_CIRCLE);
}

void RendererDebug::renderBounds(glm::vec3 center, const Bounds &bounds) {
   positions.push_back(center + glm::vec3(bounds.min_x, bounds.min_y, bounds.min_z));
   positions.push_back(center + glm::vec3(bounds.max_x, bounds.max_y, bounds.max_z));
   shapes.push_back(SHAPE_BOX);
   shapes.push_back(SHAPE_BOX);
}

void RendererDebug::log(std::string text) {
   debug_log.push_back(text);
}

void RendererDebug::renderLog() {
   if (debug_log.size() == 0) return;
   std::vector<glm::vec2> positions, uvs;
   
   float font_size = 32.0f;
   float font_width = font_size / w_width;
   float font_height = font_size / w_height;
   for (int i = 0; i < debug_log.size(); i ++) {
      glm::vec2 text_pos = glm::vec2(1 - font_width * debug_log[i].length(),
                                     1 - i * font_height);
      
      for (int c = 0; c < debug_log[i].length(); c ++) {
         positions.push_back(text_pos);
         positions.push_back(text_pos + glm::vec2(font_width, -font_height));
         
         char character = debug_log[i][c];
         float uv_x = (character%16)/16.0f;
         float uv_y = (character/16)/16.0f;
         uvs.push_back(glm::vec2(uv_x, uv_y));
         uvs.push_back(glm::vec2(uv_x, uv_y) + glm::vec2(1.0f / 16.0f));
         
         text_pos.x += font_width;
      }
   }
   
   log_renderer->setNumElements(positions.size());
   log_renderer->bufferData(Vertices, positions);
   log_renderer->bufferData(UVs, uvs);
   
   log_renderer->render(glm::mat4(1.0f));
   
   debug_log.clear();
}

void RendererDebug::render(glm::mat4 model) {
   GLenum error = glGetError();
   assert(error == 0);
   
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_vertex]);
   glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_shape]);
   glBufferData(GL_ARRAY_BUFFER, shapes.size() * sizeof(int), &shapes[0], GL_STATIC_DRAW);
   
   glUseProgram(program);
   
   // Send window scale
   if(w_width > w_height)
      glUniform2f(uWinScale, (float) w_height / w_width, 1);
   else
      glUniform2f(uWinScale, 1, (float) w_width / w_height);
   
   // Send projection and view matrices
   glm::mat4 View = camera_getMatrix();
   glm::mat4 Proj = renderer_getProjection();
   glUniformMatrix4fv(uView,  1, GL_FALSE, &View [0][0]);
   glUniformMatrix4fv(uProj,  1, GL_FALSE, &Proj [0][0]);
   
   error = glGetError();
   assert(error == 0);
   
   // Bind attributes...
   glEnableVertexAttribArray(LOCATION_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_vertex]);
   glVertexAttribPointer(LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glEnableVertexAttribArray(LOCATION_SHAPE);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_shape]);
   glVertexAttribPointer(LOCATION_SHAPE, 1, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Draw it!
   glDrawArrays(GL_LINES, 0, positions.size());
   
   // Empty debug arrays
   positions.clear();
   shapes.clear();
   
   // Cleanup
   glDisableVertexAttribArray(LOCATION_POSITION);
   glDisableVertexAttribArray(LOCATION_SHAPE);
   
   glUseProgram(0);
   
   error = glGetError();
   assert(error == 0);
}