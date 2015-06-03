//
//  renderer_2D.cpp
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
#include "Renderer2D.h"

GLuint Renderer2D::program, Renderer2D::uWinScale, Renderer2D::uProj;
GLuint Renderer2D::uModel, Renderer2D::uHasOpacity;
GLuint Renderer2D::uTexture, Renderer2D::uZValue;

bool Renderer2D::initialized = false;
void Renderer2D::init() {
   program = LoadShaders("./shaders/2DVertex.glsl", "./shaders/2DGeometry.glsl", "./shaders/2DFragment.glsl");
   uWinScale = glGetUniformLocation(program, "windowScale");
   uProj = glGetUniformLocation(program, "uProjMatrix");
   uModel = glGetUniformLocation(program, "uModelMatrix");
   uTexture = glGetUniformLocation(program, "uTexture");
   uZValue = glGetUniformLocation(program, "uZValue");
   uHasOpacity = glGetUniformLocation(program, "hasOpacity");
   
   initialized = true;
}

Renderer2D::Renderer2D(std::string textureName, bool o, float z) : Renderer(), b_vertex(Vertices), b_uv(UVs), b_opacity(Opacities), z(z), elements(0), hasOpacity(o) {
   if (!initialized)
      init();

   b_vertex.init();
   b_uv.init();
   if (hasOpacity)
      b_opacity.init();
   
   texture = texture_load(textureName);
   
   GLenum error = glGetError();
   assert(error == 0);
};

void Renderer2D::render(glm::mat4 Model) {
   GLenum error = glGetError();
   assert(error == 0);
   
   glUseProgram(program);
   
   // Send window scale
   if(w_width > w_height)
      glUniform2f(uWinScale, (float) w_height / w_width, 1);
   else
      glUniform2f(uWinScale, 1, (float) w_width / w_height);
   
   glm::mat4 Proj(1.0f);
   
   glUniformMatrix4fv(uProj,  1, GL_FALSE, &Proj [0][0]);
   glUniformMatrix4fv(uModel, 1, GL_FALSE, &Model[0][0]);
   
   glUniform1f(uZValue, z);
   glUniform1i(uHasOpacity, hasOpacity);
   
   error = glGetError();
   assert(error == 0);
      
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);
   glUniform1i(uTexture, 0);
   
   error = glGetError();
   assert(error == 0);
   
   // Bind attributes...
   b_vertex.attribPointer(LOCATION_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
   b_uv.attribPointer(LOCATION_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
   if (hasOpacity)
      b_opacity.attribPointer(LOCATION_OPACITY, 1, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Draw it!
   glDrawArrays(GL_LINES, 0, elements);
   
   if(elements == 0)
      std::cerr << "WARNING: Rendering an object with 0 elements" << std::endl;
   
   // Cleanup
   glDisableVertexAttribArray(LOCATION_POSITION);
   glDisableVertexAttribArray(LOCATION_UV);
   glDisableVertexAttribArray(LOCATION_OPACITY);
   
   glUseProgram(0);
   
   error = glGetError();
   assert(error == 0);
}

void Renderer2D::bufferData(DataType type, size_t size, void *data) {
   VBO *buf;

   if (type == Vertices) {
      buf = &b_vertex;
   }
   else if (type == Opacities) {
      buf = &b_opacity;
   }
   else if (type == UVs) {
      buf = &b_uv;
   }
   else {
      std::cerr << "Buffer type " << type << " not recognized" << std::endl;
      assert(0);
   }

   buf->bufferData(size, data, GL_STATIC_DRAW);
}

void Renderer2D::bufferData(DataType type, const std::vector<float> &data) {
   assert(type == Vertices || type == UVs || type == Opacities);
   if (type == Vertices)
      elements = data.size() * 2;
   
   bufferData(type, sizeof(float) * data.size(), (void *)&data[0]);
}

void Renderer2D::bufferData(DataType type, const std::vector<glm::vec2> &data) {
   assert(type == Vertices || type == UVs);
   if (type == Vertices)
      elements = data.size();
   
   bufferData(type, sizeof(glm::vec2) * data.size(), (void *)&data[0]);
}