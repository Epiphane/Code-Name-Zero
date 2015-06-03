//  texture.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/23/15.
//
//

#ifndef __RGBZero__texture__
#define __RGBZero__texture__

#include <glm/glm.hpp>
#include "GLSL.h"
#include <string>

#define MAX_TEXTURE_SIZE 512

GLuint texture_load(std::string filename);
GLvoid texture_loadToArray(std::string filename, int texture, int layer, int *width, int *height);

class RandomTexture {
public:
    RandomTexture();
    
    ~RandomTexture();
    
    bool InitRandomTexture(unsigned int Size);
    
    void Bind(GLenum TextureUnit);
    
private:
    GLuint m_textureObj;
};
class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);
    
    bool Load();
    
    void Bind(GLenum TextureUnit);
    
private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
};

#endif /* defined(__RGBZero__texture__) */
