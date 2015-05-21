//  particle_system.cpp
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#include "particle_system.h"
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "main.h"
#include "shader.hpp"

using namespace std;

struct Particle
{
    float Type;
    glm::vec3 Pos;
    glm::vec3 Vel;
    float LifetimeMillis;
};

ParticleSystem::ParticleSystem(void) {
    //Initialize the switch flags for the buffers
    m_currTFB = 1;
    m_currVB = 0;
    m_isFirst = true;
    m_time = 0;
    m_pTexture = NULL;
    
    ZERO_MEM(m_transformFeedback);
    ZERO_MEM(m_particleBuffer);
}

bool ParticleSystem::InitParticleSystem(glm::vec3 Position) {
    Particle Particles[MAX_PARTICLES];
    ZERO_MEM(Particles);
    
    //Initial particle considered the emitter
    Particles[0].Type = PARTICLE_TYPE_LAUNCHER;
    Particles[0].Pos = Position;
    Particles[0].Vel = glm::vec3(0.0f);//, 0.0001f, 0.0f);
    Particles[0].LifetimeMillis = 0.0f;
    
    glGenTransformFeedbacks(2, m_transformFeedback);
    glGenBuffers(2, m_particleBuffer);
    
    for (unsigned int i = 0; i < 2; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
    }
    
    //Set up how the particle system should update itself
    if (!m_updateTechnique.Init()) {
        fprintf(stderr, "updateTechnique failed in particle_system");
        return false;
    }
    
    m_updateTechnique.Enable();
    
    m_updateTechnique.SetRandomTextureUnit(3);
    
    //Times are set in milliseconds
    m_updateTechnique.SetLauncherLifetime(8.0f);
    m_updateTechnique.SetShellLifetime(500.0f);
    //m_updateTechnique.SetSecondaryShellLifetime(2500.0f);
    
    if (!m_randomTexture.InitRandomTexture(1000)) {
        fprintf(stderr, "randomTexture failed in particle_system");
        return false;
    }
    
    m_randomTexture.Bind(GL_TEXTURE3);
    
    //Set up Billboarding (GS creates points, billboard creates quads)
    if (!m_billboardTechnique.Init()) {
        fprintf(stderr, "billboard failed in particle_system");
        return false;
    }
    
    m_billboardTechnique.Enable();
    
    m_billboardTechnique.SetColorTextureUnit(0);
    m_billboardTechnique.SetBillboardSize(.05f);
    
    m_pTexture = new Texture(GL_TEXTURE_2D, "./textures/smoke1.png");
    
    if (!m_pTexture->Load()) {
        fprintf(stderr, "textures failed in particle_system");
        return false;
    }
    
    return true;
}

void ParticleSystem::UpdateParticles(int DeltaTimeMillis)
{
    m_time += DeltaTimeMillis;
    m_updateTechnique.Enable();
    m_updateTechnique.SetTime(m_time);
    m_updateTechnique.SetDeltaTimeMillis(DeltaTimeMillis);
    
    m_randomTexture.Bind(GL_TEXTURE3);
    
    glEnable(GL_RASTERIZER_DISCARD);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);                          // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);         // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);        // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);          // lifetime
    
    glBeginTransformFeedback(GL_POINTS);
    
    if (m_isFirst == 1) {
        glDrawArrays(GL_POINTS, 0, 1);
        
        m_isFirst = 2;
    }
    else {
        m_isFirst ++;
        glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
    }
    
    glEndTransformFeedback();
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    
    glDisable(GL_RASTERIZER_DISCARD);
}

void ParticleSystem::RenderParticles(const glm::mat4 &View, const glm::mat4 &Projection, const glm::vec3 &CameraPos)
{
    m_billboardTechnique.Enable();
    m_billboardTechnique.SetCameraPosition(CameraPos);
    m_billboardTechnique.setView(View);
    m_billboardTechnique.setProjection(Projection);
    m_pTexture->Bind(GL_TEXTURE0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);
    
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);  // position
    
    glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);
    
    glDisableVertexAttribArray(0);
    
    //Switch buffers every Render call
    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
}



