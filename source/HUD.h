//
//  HUD.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/28/15.
//
//

#ifndef __RGBZero__HUD__
#define __RGBZero__HUD__

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "renderer2D.h"

class InGameState;
class GameObject;

class HUD {
private:
   long speed, score;
   
   const float HUD_TEXTURE_SIZE = 512.0f;
   
   static unsigned int buf_progress_ndx;
   static float progress_bar_height;
   
   Renderer2D *renderer;
   
   std::vector<glm::vec2> posBuf, uvBuf;
   
public:
   HUD();
   
   void update(float dt, InGameState *state);
   void render(float dt);
};

#endif /* defined(__RGBZero__HUD__) */
