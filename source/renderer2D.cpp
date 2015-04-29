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
GLuint Renderer2D::uModel, Renderer2D::aUV, Renderer2D::aPosition;
GLuint Renderer2D::uTexture;

bool Renderer2D::initialized = false;
void Renderer2D::init() {
   program = LoadShaders("./shaders/2DVertex.glsl", "./shaders/2DGeometry.glsl", "./shaders/2DFragment.glsl");
   uWinScale = glGetUniformLocation(program, "windowScale");
   uProj = glGetUniformLocation(program, "uProjMatrix");
   uModel = glGetUniformLocation(program, "uModelMatrix");
   aPosition = glGetAttribLocation(program, "aPosition");
   aUV = glGetAttribLocation(program, "aUV");
   uTexture = glGetUniformLocation(program, "uTexture");
   
   initialized = true;
}

Renderer2D::Renderer2D(std::string textureName) : Renderer(0) {
   if (!initialized)
      init();
   
   glGenBuffers(NUM_BUFFERS, buffers);
   
   glGenTextures(1, &texture);
   texture_load(textureName, texture);
   
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
   
   error = glGetError();
   assert(error == 0);
      
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);
   glUniform1i(uTexture, 0);
   
   error = glGetError();
   assert(error == 0);
   
   // Bind attributes...
   glEnableVertexAttribArray(LOCATION_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_vertex]);
   glVertexAttribPointer(LOCATION_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
   glEnableVertexAttribArray(LOCATION_UV);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_uv]);
   glVertexAttribPointer(LOCATION_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Draw it!
   glDrawArrays(GL_LINES, 0, elements);
   
   if(elements == 0)
      std::cerr << "WARNING: Rendering an object with 0 elements" << std::endl;
   
   // Cleanup
   glDisableVertexAttribArray(LOCATION_POSITION);
   glDisableVertexAttribArray(LOCATION_UV);
   
   glUseProgram(0);
   
   error = glGetError();
   assert(error == 0);
}

void Renderer2D::bufferData(DataType type, size_t size, void *data) {
   GLuint bufType;
   
   if(type == Vertices) {
      bufType = GL_ARRAY_BUFFER;
      
      glBindBuffer(bufType, buffers[b_vertex]);
   }
   else if(type == UVs) {
      bufType = GL_ARRAY_BUFFER;
      
      glBindBuffer(bufType, buffers[b_uv]);
   }
   else {
      std::cerr << "Buffer type " << type << " not recognized" << std::endl;
      assert(0);
   }
   
   glBufferData(bufType, size, data, GL_STATIC_DRAW);
}

void Renderer2D::bufferData(DataType type, const std::vector<float> &data) {
   assert(type == Vertices || type == UVs);
   bufferData(type, sizeof(float) * data.size(), (void *)&data[0]);
}

void Renderer2D::bufferData(DataType type, const std::vector<glm::vec2> &data) {
   assert(type == Vertices || type == UVs);
   bufferData(type, sizeof(glm::vec2) * data.size(), (void *)&data[0]);
}