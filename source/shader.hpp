#ifndef SHADER_HPP
#define SHADER_HPP

GLuint LoadShaders(const char *vertFilePath,const char *fragFilePath);
GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath);

#endif
