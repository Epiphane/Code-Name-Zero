//
//  vertex_buffer_object.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 5/7/15.
//
//

#include <vector>

#include "renderer.h"
#include "vertex_buffer_object.h"

#define BUFFER_COUNT_INCREMENT 256
std::vector<int> buffer_references(BUFFER_COUNT_INCREMENT);

void set_buffer_reference_1(GLuint buffer) {
   if (buffer_references.size() < buffer) {
      buffer_references.resize(buffer_references.size() + BUFFER_COUNT_INCREMENT);
   }
   
   buffer_references[buffer] = 1;
}

// Constructors
VBO::VBO(DataType _type, GLuint _buffer) {
   if (_type == Indices) bufferType = GL_ELEMENT_ARRAY_BUFFER;
   else bufferType = GL_ARRAY_BUFFER;
   
   buffer = _buffer;
   buffer_references[buffer] ++;
}

VBO::VBO(DataType _type) {
   if (_type == Indices) bufferType = GL_ELEMENT_ARRAY_BUFFER;
   else bufferType = GL_ARRAY_BUFFER;
   
   glGenBuffers(1, &buffer);
   set_buffer_reference_1(buffer);
};

// Buffer data
void VBO::bufferData(GLsizei size, void *data, GLuint strategy) {
   // Shared buffer!
   if (buffer_references[buffer] > 1) {
      buffer_references[buffer] --;
      
      glGenBuffers(1, &buffer);
      set_buffer_reference_1(buffer);
   }
   
   glBindBuffer(bufferType, buffer);
   glBufferData(bufferType, size, data, strategy);
}

void VBO::bind() {
   glBindBuffer(bufferType, buffer);
}

void VBO::attribPointer(GLuint location, GLint count, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
   glEnableVertexAttribArray(location);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glVertexAttribPointer(location, count, type, normalized, stride, pointer);
}