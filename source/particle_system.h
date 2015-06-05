//  particle_system.h
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#ifndef __RGBZero__particle_system__
#define __RGBZero__particle_system__


#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string.h>

#include "ps_update_technique.h"
#include "billboard_technique.h"
#include "texture.h"

using namespace std;

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define MAX_PARTICLES 10000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f

void Particles();

class ParticleSystem {
public:
   ParticleSystem();
   ~ParticleSystem();
    
    bool InitParticleSystem(glm::vec3 Position);
    void Render(int DeltaTimeMillis, const glm::mat4 &View, const glm::mat4 &Projection, const glm::vec3 &CameraPos);
    void UpdateParticles(int DeltaTimeMillis, float playerSpeed);
    void RenderParticles(const glm::mat4 &View, const glm::mat4 &Projection, const glm::vec3 &CameraPos);

private:
    int m_isFirst; // Indicator if Render has been called
    unsigned int m_currVB;
    unsigned int m_currTFB;
    GLuint m_particleBuffer[2];
    GLuint m_transformFeedback[2];
    PSUpdateTechnique m_updateTechnique;
    BillboardTechnique m_billboardTechnique;
    RandomTexture m_randomTexture;
    Texture* m_pTexture;
    int m_time;
   
   static int transform_feedback_supported;
};

#endif /* defined(__RGBZero__particle_system__) */
