//
//  HUD.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/28/15.
//
//

#include "in_game_state.h"
#include "game_object.h"
#include "HUD.h"
#include "texture.h"

unsigned int HUD::buf_progress_ndx, HUD::mph_digit_ndx, HUD::score_digit_ndx;
float HUD::progress_bar_height;

HUD::HUD() {
   posBuf.clear();
   uvBuf.clear();
   
   // Progress bar
   /*progress_bar_height = 13.0f / HUD_TEXTURE_SIZE;
   buf_progress_ndx = posBuf.size();
   posBuf.push_back(glm::vec2(-1, 1));
   posBuf.push_back(glm::vec2(1, 1 - progress_bar_height));
   uvBuf.push_back(glm::vec2(0));
   uvBuf.push_back(glm::vec2(1, progress_bar_height));
   */
   // MPH Marker
   const float mph_marker_th = 19.0f / HUD_TEXTURE_SIZE;
   const float mph_marker_tw = 92.0f / HUD_TEXTURE_SIZE;
   const float mph_marker_ty = 48.0f / HUD_TEXTURE_SIZE;
   const float mph_marker_x = -0.97f;
   const float mph_marker_y = -0.97f;
   posBuf.push_back(glm::vec2(mph_marker_x, mph_marker_th + mph_marker_y));
   posBuf.push_back(glm::vec2(mph_marker_tw + mph_marker_x, mph_marker_y));
   uvBuf.push_back(glm::vec2(0, mph_marker_ty));
   uvBuf.push_back(glm::vec2(mph_marker_tw, mph_marker_ty + mph_marker_th));
   
   // MPH Digits
   const float mph_digit_th = 16.0f / HUD_TEXTURE_SIZE;
   const float mph_digit_tw = 16.0f / HUD_TEXTURE_SIZE;
   const float mph_digit_tx = 0;
   const float mph_digit_ty = 16.0f / HUD_TEXTURE_SIZE;
   mph_digit_ndx = posBuf.size();
   for (int i = 0; i < MAX_SPEED_DIGITS; i ++) {
      posBuf.push_back(glm::vec2(i * mph_digit_tw + mph_marker_x,
                                 mph_marker_y + mph_marker_th + mph_digit_th));
      posBuf.push_back(glm::vec2((i + 1) * mph_digit_tw + mph_marker_x,
                                 mph_marker_y + mph_marker_th));
      uvBuf.push_back(glm::vec2(mph_digit_tx, mph_digit_ty));
      uvBuf.push_back(glm::vec2(mph_digit_tx + mph_digit_tw, mph_digit_ty + mph_digit_th));
   }
   
   // Score marker
   const float score_marker_th = 0;//19.0f / HUD_TEXTURE_SIZE;
   const float score_marker_tw = 0;//92.0f / HUD_TEXTURE_SIZE;
   const float score_marker_ty = 0;//48.0f / HUD_TEXTURE_SIZE;
   
   // Score Digits
   const float score_digit_th = 16.0f / HUD_TEXTURE_SIZE;
   const float score_digit_tw = 16.0f / HUD_TEXTURE_SIZE;
   const float score_digit_tx = 0;
   const float score_digit_ty = 16.0f / HUD_TEXTURE_SIZE;
   const float score_marker_x = 0.0f;
   const float score_marker_y = 0.79f;
   score_digit_ndx = posBuf.size();
   for (int i = 0; i < MAX_SCORE_DIGITS; i ++) {
      posBuf.push_back(glm::vec2(i * score_digit_tw + score_marker_x,
                                 score_marker_y + score_marker_th + score_digit_th));
      posBuf.push_back(glm::vec2((i + 1) * score_digit_tw + score_marker_x,
                                 score_marker_y + score_marker_th));
      
      uvBuf.push_back(glm::vec2(score_digit_tx, score_digit_ty));
      uvBuf.push_back(glm::vec2(score_digit_tx + score_digit_tw, score_digit_ty + score_digit_th));
   }
   
   renderer = new Renderer2D("./textures/HUD.png");
   
   renderer->setNumElements(posBuf.size());
   renderer->bufferData(Vertices, posBuf);
   renderer->bufferData(UVs, uvBuf);
}

void HUD::update(float dt, InGameState *state) {
   this->speed = state->getPlayerSpeed();
   int speed = this->speed;
   
   // Write speed
   assert(speed < MAX_SPEED);
   bool num_written = false;
   int digit_factor = MAX_SPEED / 10;
   for (int i = 0; i < MAX_SPEED_DIGITS; i ++) {
      int ndx = mph_digit_ndx + 2 * i;
      if (speed / digit_factor > 0) {
         num_written = true;
         int digit = speed / digit_factor;
         
         uvBuf[ndx]     = glm::vec2(digit * 16.0f / HUD_TEXTURE_SIZE,
                                    16.0f / HUD_TEXTURE_SIZE);
         uvBuf[ndx + 1] = glm::vec2((digit + 1) * 16.0f / HUD_TEXTURE_SIZE,
                                    32.0f / HUD_TEXTURE_SIZE);
         
         if (uvBuf[ndx].x >= 1) {
            uvBuf[ndx].x     -= 1;
            uvBuf[ndx + 1].x -= 1;
            uvBuf[ndx].y     += 16.0f / HUD_TEXTURE_SIZE;
            uvBuf[ndx + 1].y += 16.0f / HUD_TEXTURE_SIZE;
         }
      }
      else if (num_written) {
         uvBuf[ndx]     = glm::vec2(32.0f / HUD_TEXTURE_SIZE);
         uvBuf[ndx + 1] = glm::vec2(48.0f / HUD_TEXTURE_SIZE);
      }
      else {
         uvBuf[ndx]     = glm::vec2(0, 16.0f / HUD_TEXTURE_SIZE);
         uvBuf[ndx + 1] = glm::vec2(16.0f / HUD_TEXTURE_SIZE,
                                    32.0f / HUD_TEXTURE_SIZE);
      }
      
      speed = speed % digit_factor;
      digit_factor /= 10;
   }
   
   // Write score
   score = state->getScore();
   assert(score < MAX_SCORE);
   num_written = false;
   digit_factor = MAX_SCORE / 10;
   for (int i = 0; i < MAX_SCORE_DIGITS; i ++) {
      int ndx = score_digit_ndx + 2 * i;
      if (score / digit_factor > 0) {
         num_written = true;
         int digit = score / digit_factor;
         
         uvBuf[ndx]     = glm::vec2(digit * 16.0f / HUD_TEXTURE_SIZE,
                                    16.0f / HUD_TEXTURE_SIZE);
         uvBuf[ndx + 1] = glm::vec2((digit + 1) * 16.0f / HUD_TEXTURE_SIZE,
                                    32.0f / HUD_TEXTURE_SIZE);
         
         if (uvBuf[ndx].x >= 1) {
            uvBuf[ndx].x     -= 1;
            uvBuf[ndx + 1].x -= 1;
            uvBuf[ndx].y     += 16.0f / HUD_TEXTURE_SIZE;
            uvBuf[ndx + 1].y += 16.0f / HUD_TEXTURE_SIZE;
         }
      }
      else if (num_written) {
         uvBuf[ndx]     = glm::vec2(32.0f / HUD_TEXTURE_SIZE);
         uvBuf[ndx + 1] = glm::vec2(48.0f / HUD_TEXTURE_SIZE);
      }
      else {
         uvBuf[ndx]     = glm::vec2(0, 16.0f / HUD_TEXTURE_SIZE);
         uvBuf[ndx + 1] = glm::vec2(16.0f / HUD_TEXTURE_SIZE,
                                    32.0f / HUD_TEXTURE_SIZE);
      }
      
      score = score % digit_factor;
      digit_factor /= 10;
   }
   
   // Show percent done
   /*float percentDone = state->getSoundtrack()->getProgress();
   if (percentDone < 1) {
      percentDone += dt / 2;
      
      posBuf[buf_progress_ndx + 1] = glm::vec2(2*percentDone - 1, 1 - progress_bar_height);
      uvBuf[buf_progress_ndx + 1] = glm::vec2(percentDone, progress_bar_height);
      
      renderer->bufferData(Vertices, posBuf);
   }*/
   
   renderer->bufferData(UVs, uvBuf);
}

void HUD::render(float dt) {
   renderer->render(glm::mat4(1.0f));
}