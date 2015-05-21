//
//  technique.h
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#ifndef __RGBZero__technique__
#define __RGBZero__technique__

#include <list>
#include <GL/glew.h>

#define INVALID_UNIFORM_LOCATION 0xffffffff

class Technique
{
public:
    
    Technique();
    
    virtual ~Technique();
    
    virtual bool Init();
    
    void Enable();
    
protected:
    
    bool AddShader(GLenum ShaderType, const char* pFilename);
    
    bool Finalize();
    
    GLint GetUniformLocation(const char* pUniformName);
    
    GLint GetProgramParam(GLint param);
    
    GLuint m_shaderProg;
    
private:
    
    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
};


#endif /* defined(__RGBZero__technique__) */
