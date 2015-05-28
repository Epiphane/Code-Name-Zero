//  ps_update_technique.h
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#ifndef __RGBZero__ps_update_technique__
#define __RGBZero__ps_update_technique__

#include <stdio.h>

#include "technique.h"

class PSUpdateTechnique : public Technique
{
public:
   PSUpdateTechnique();
   
   virtual bool Init();
   
   void SetParticleLifetime(float Lifetime);
   
   void SetDeltaTimeMillis(unsigned int DeltaTimeMillis);
   
   void SetTime(int Time);
   
   void SetRandomTextureUnit(unsigned int TextureUnit);
   
   void SetLauncherLifetime(float Lifetime);
   
   void SetShellLifetime(float Lifetime);
   
   void SetPlayerSpeed(float Speed);
   
   //    void SetSecondaryShellLifetime(float Lifetime);
   
   
private:
   GLuint m_deltaTimeMillisLocation;
   GLuint m_randomTextureLocation;
   GLuint m_timeLocation;
   GLuint m_launcherLifetimeLocation;
   GLuint m_shellLifetimeLocation;
   GLuint m_playerSpeedLocation;
   //    GLuint m_secondaryShellLifetimeLocation;
};

#endif /* defined(__RGBZero__ps_update_technique__) */
