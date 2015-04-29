//
//  HUD.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/28/15.
//
//

#include "game_object.h"
#include "HUD.h"
#include "texture.h"

unsigned int HUD::buf_progress_ndx;
float HUD::progress_bar_height;

HUD::HUD(GameObject *player) : player(player) {
   posBuf.clear();
   uvBuf.clear();
   
   // Progress bar
   progress_bar_height = 13.0f / HUD_TEXTURE_SIZE;
   buf_progress_ndx = posBuf.size();
   posBuf.push_back(glm::vec2(-1, 1));
   posBuf.push_back(glm::vec2(1, 1 - progress_bar_height * 2));
   uvBuf.push_back(glm::vec2(0));
   uvBuf.push_back(glm::vec2(1, progress_bar_height));
   
   renderer = new Renderer2D("./textures/HUD.png");
   
   renderer->setNumElements(posBuf.size());
   renderer->bufferData(Vertices, posBuf);
   renderer->bufferData(UVs, uvBuf);
}

void HUD::update(float dt) {
   speed = 182;
   score = 21000;
   
   static float percentDone = 0;
   if (percentDone < 1) {
      percentDone += dt / 2;
      
      posBuf[buf_progress_ndx + 1] = glm::vec2(2*percentDone - 1, 1 - progress_bar_height * 2);
      uvBuf[buf_progress_ndx + 1] = glm::vec2(percentDone, progress_bar_height);
      
      renderer->bufferData(Vertices, posBuf);
      renderer->bufferData(UVs, uvBuf);
   }
}

void HUD::render(float dt) {
   renderer->render(glm::mat4(1.0f));
}