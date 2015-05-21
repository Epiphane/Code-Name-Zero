//  billboard_technique.cpp
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#include "billboard_technique.h"
#include "main.h"

BillboardTechnique::BillboardTechnique()
{
}


bool BillboardTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }
    
    if (!AddShader(GL_VERTEX_SHADER, "./shaders/billboardVertex.glsl")) {
        fprintf(stderr, "Add Shader Shader failed in billboard_update_technique");
        return false;
    }
    
    if (!AddShader(GL_GEOMETRY_SHADER, "./shaders/billboardGeometryTest.glsl")) {
        fprintf(stderr, "Add Geometry Shader failed in billboard_update_technique");
        return false;
    }
    
    if (!AddShader(GL_FRAGMENT_SHADER, "./shaders/billboardFragment.glsl")) {
        fprintf(stderr, "Add Fragment Shader failed in billboard_update_technique");
        return false;
    }
    
    if (!Finalize()) {
        fprintf(stderr, "Finalize failed in billboard_update_technique");
        return false;
    }
    
    m_viewLocation = GetUniformLocation("gView");
    m_projectionLocation = GetUniformLocation("gProjection");
    m_cameraPosLocation = GetUniformLocation("gCameraPos");
    m_colorMapLocation = GetUniformLocation("gColorMap");
    m_billboardSizeLocation = GetUniformLocation("gBillboardSize");
    
    if (m_viewLocation == INVALID_UNIFORM_LOCATION ||
        m_cameraPosLocation == INVALID_UNIFORM_LOCATION ||
        m_billboardSizeLocation == INVALID_UNIFORM_LOCATION ||
        m_colorMapLocation == INVALID_UNIFORM_LOCATION ||
        m_projectionLocation == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Invalid Uniform Locations");
        return false;
    }
    
    return glGetError() == GL_NO_ERROR;
}


void BillboardTechnique::setView(const glm::mat4 View) {
    glUniformMatrix4fv(m_viewLocation, 1, GL_TRUE, &View[0][0]);
}

void BillboardTechnique::setProjection(const glm::mat4 Projection) {
    glUniformMatrix4fv(m_projectionLocation, 1, GL_TRUE, &Projection[0][0]);
}


void BillboardTechnique::SetCameraPosition(const glm::vec3 Pos)
{
    glUniform3f(m_cameraPosLocation, Pos.x, Pos.y, Pos.z);
}


void BillboardTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorMapLocation, TextureUnit);
}


void BillboardTechnique::SetBillboardSize(float BillboardSize)
{
    glUniform1f(m_billboardSizeLocation, BillboardSize);
}
