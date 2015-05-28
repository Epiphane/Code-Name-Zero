//  ps_update_technique.cpp
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#include "ps_update_technique.h"
#include "technique.h"
#include <cassert>
#include <iostream>

PSUpdateTechnique::PSUpdateTechnique()
{
   
}


bool PSUpdateTechnique::Init()
{
   if (!Technique::Init()) {
      return false;
   }
   
   if (!AddShader(GL_VERTEX_SHADER, "./shaders/ps_updateVertex.glsl")) {
      fprintf(stderr, "Add Vertex Shader failed in ps_update_technique");
      return false;
   }
   
   if (!AddShader(GL_GEOMETRY_SHADER, "./shaders/ps_updateGeometry.glsl")) {
      fprintf(stderr, "Add Geometry Shader failed in ps_update_technique");
      return false;
   }
   
   if (!AddShader(GL_FRAGMENT_SHADER, "./shaders/ps_updateFragment.glsl")) {
      fprintf(stderr, "Add Fragment Shader failed in ps_update_technique");
      return false;
   }
   
   if (!Finalize()) {
      fprintf(stderr, "Finalize failed in ps_update_technique");
      return false;
   }
   
   const GLchar* Varyings[4];
   Varyings[0] = "Type1";
   Varyings[1] = "Position1";
   Varyings[2] = "Velocity1";
   Varyings[3] = "Age1";
   
   glTransformFeedbackVaryings(m_shaderProg, 4, Varyings, GL_INTERLEAVED_ATTRIBS);
   GLenum error = glGetError();
   assert(error == 0);
   
   if (!Finalize()) {
      fprintf(stderr, "Finalize failed in ps_update_technique");
      return false;
   }
   
   m_deltaTimeMillisLocation = GetUniformLocation("gDeltaTimeMillis");
   m_randomTextureLocation = GetUniformLocation("gRandomTexture");
   m_timeLocation = GetUniformLocation("gTime");
   m_launcherLifetimeLocation = GetUniformLocation("gLauncherLifetime");
   m_shellLifetimeLocation = GetUniformLocation("gShellLifetime");
   m_playerSpeedLocation = GetUniformLocation("gPlayerSpeed");
   //m_secondaryShellLifetimeLocation = GetUniformLocation("gSecondaryShellLifetime");
   
   if (m_deltaTimeMillisLocation == INVALID_UNIFORM_LOCATION ||
       m_timeLocation == INVALID_UNIFORM_LOCATION ||
       m_randomTextureLocation == INVALID_UNIFORM_LOCATION ||
       m_launcherLifetimeLocation == INVALID_UNIFORM_LOCATION ||
       m_shellLifetimeLocation == INVALID_UNIFORM_LOCATION ||
       m_playerSpeedLocation == INVALID_UNIFORM_LOCATION
       /*m_secondaryShellLifetimeLocation == INVALID_UNIFORM_LOCATION*/) {
      fprintf(stderr, "Invalid Uniform Locations");
      return false;
   }
   
   return true;
}

void PSUpdateTechnique::SetDeltaTimeMillis(unsigned int DeltaTimeMillis)
{
   glUniform1f(m_deltaTimeMillisLocation, (float)DeltaTimeMillis);
}


void PSUpdateTechnique::SetTime(int Time)
{
   glUniform1f(m_timeLocation, (float)Time);
}


void PSUpdateTechnique::SetRandomTextureUnit(unsigned int TextureUnit)
{
   glUniform1i(m_randomTextureLocation, TextureUnit);
}


void PSUpdateTechnique::SetLauncherLifetime(float Lifetime)
{
   glUniform1f(m_launcherLifetimeLocation, Lifetime);
}


void PSUpdateTechnique::SetShellLifetime(float Lifetime)
{
   glUniform1f(m_shellLifetimeLocation, Lifetime);
}

void PSUpdateTechnique::SetPlayerSpeed(float Speed)
{
   glUniform1f(m_playerSpeedLocation, Speed);
}

//void PSUpdateTechnique::SetSecondaryShellLifetime(float Lifetime)
//{
//    glUniform1f(m_secondaryShellLifetimeLocation, Lifetime);
//}
