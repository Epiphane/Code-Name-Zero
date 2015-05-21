//  billboard_technique.h
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#ifndef __RGBZero__billboard_technique__
#define __RGBZero__billboard_technique__

#include <stdio.h>
#include <glm/glm.hpp>
#include "technique.h"

class BillboardTechnique : public Technique
{
public:
    
    BillboardTechnique();
    
    virtual bool Init();
    
    void setView(const glm::mat4 View);
    void setProjection(const glm::mat4 Projection);
    void SetCameraPosition(const glm::vec3 Pos);
    void SetColorTextureUnit(unsigned int TextureUnit);
    void SetBillboardSize(float BillboardSize);
    
private:
    
    GLuint m_cameraPosLocation;
    GLuint m_colorMapLocation;
    GLuint m_billboardSizeLocation;
    GLuint m_projectionLocation;
    GLuint m_viewLocation;
};

#endif /* defined(__RGBZero__billboard_technique__) */
