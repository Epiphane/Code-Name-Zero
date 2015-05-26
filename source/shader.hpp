#ifndef SHADER_HPP
#define SHADER_HPP

#include "GLSL.h"

GLuint LoadShaders(const char *vertFilePath,const char *fragFilePath);
GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath);

#endif
