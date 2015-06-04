//
//  vertex_buffer_object.h
//  RGBZero
//
//  Created by Thomas Steinke on 5/7/15.
//
//

#ifndef __RGBZero__vertex_buffer_object__
#define __RGBZero__vertex_buffer_object__

#include "GLSL.h"
#include "renderer.h"

class VBO {
private:
   GLuint buffer, bufferType;
   
public:
   VBO(DataType type);
   VBO(DataType type, GLuint buffer);
   VBO(const VBO& other);
   ~VBO();
   void init();
   
   VBO& operator=(const VBO& other);
   
   void bind();
   void attribPointer(GLuint location, GLint count, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
   
   GLuint getBuffer() { return buffer; }
   void bufferData(GLsizei size, void *data, GLuint type);
};


#endif /* defined(__RGBZero__vertex_buffer_object__) */
