//
//  rendererSky.h
//  RGBZero
//
//  Created by Max Linsenbard on 5/25/15.
//
//

#ifndef __RGBZero__rendererSky__
#define __RGBZero__rendererSky__

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "tiny_obj_loader.h"
#include "renderer.h"
#include "GLSL.h"

class SkyRenderer{
private:
   GLuint program;
   GLuint uInvProjMatrix, uInvViewMatrix, uLightDir, uViewport, uFramebufferResolution, uBrightness;
   GLuint uMusicSamples;
   
public:
   SkyRenderer();
   virtual ~SkyRenderer();
   
   void render(glm::vec3 lightDirection, float brightness, float *samples);
};


#endif /* defined(__RGBZero__rendererSky__) */
