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
#include "main.h"

class InGameState;
class GameObject;

class HUD {
private:
   long speed, score;
   
   const float HUD_TEXTURE_WIDTH = 2048.0f;
   const float HUD_TEXTURE_HEIGHT = 512.0f;
   const float mph_marker_th = 179.0f / HUD_TEXTURE_HEIGHT;
   const float mph_marker_tw = 863.0f / HUD_TEXTURE_WIDTH;
   const float mph_marker_tx = 0.0f / HUD_TEXTURE_WIDTH;
   const float mph_marker_ty = 333.0f / HUD_TEXTURE_HEIGHT;
   const float mph_marker_x = -1.0f;
   const float mph_marker_y = -1.0f;
   const float mph_marker_w = 2 * mph_marker_tw * HUD_TEXTURE_WIDTH / w_width;
   const float mph_marker_h = mph_marker_w * (mph_marker_th * HUD_TEXTURE_HEIGHT) / (mph_marker_tw * HUD_TEXTURE_WIDTH);

   const float mph_bar_tx = 0.0f / HUD_TEXTURE_WIDTH;
   const float mph_bar_ty = 261.0f / HUD_TEXTURE_HEIGHT;
   const float mph_bar_tw = 809.0f / HUD_TEXTURE_WIDTH;
   const float mph_bar_th = 59.0f / HUD_TEXTURE_HEIGHT;
   const float mph_bar_padding_x = 2 * 14.0f / w_width;
   const float mph_bar_padding_y = 2 * 10.0f / w_height;
   const float mph_bar_x = mph_marker_x + mph_bar_padding_x;
   const float mph_bar_y = mph_marker_y + mph_bar_padding_y;
   const float mph_bar_w = 2 * mph_bar_tw * HUD_TEXTURE_WIDTH / w_width;
   const float mph_bar_h = mph_bar_w * (mph_bar_th * HUD_TEXTURE_HEIGHT) / (mph_bar_tw * HUD_TEXTURE_WIDTH);

   const float score_marker_th = 99.0f / HUD_TEXTURE_HEIGHT;
   const float score_marker_tw = 800.0f / HUD_TEXTURE_WIDTH;
   const float score_marker_tx = 0.0f / HUD_TEXTURE_WIDTH;
   const float score_marker_ty = 128.0f / HUD_TEXTURE_HEIGHT;
   const float score_marker_w = 2 * score_marker_tw * HUD_TEXTURE_WIDTH / w_width;
   const float score_marker_h = score_marker_w * (score_marker_th * HUD_TEXTURE_HEIGHT) / (score_marker_tw * HUD_TEXTURE_WIDTH);
   const float score_marker_x = 1.0f - score_marker_w;
   const float score_marker_y = 1.0f - score_marker_h;

   const float digit_tw = 64.0f / HUD_TEXTURE_WIDTH;
   const float digit_th = 72.0f / HUD_TEXTURE_HEIGHT;
   const float digit_tx = 0;
   const float digit_ty = 26.0f / HUD_TEXTURE_HEIGHT;
   const float digit_w = 2 * digit_tw * HUD_TEXTURE_WIDTH / w_width;
   const float digit_h = digit_w * (digit_th * HUD_TEXTURE_HEIGHT) / (digit_tw * HUD_TEXTURE_WIDTH);
   const float mph_digit_x = mph_marker_x + 2 * 218.0f / w_width;
   const float mph_digit_y = mph_marker_y + 2 * 70.0f / w_height;
   const float score_digit_x = score_marker_x + 2 * 318.0f / w_width;
   const float score_digit_y = score_marker_y + 2 * 10.0f / w_height;

   const float combo_marker_th = 100.0f / HUD_TEXTURE_HEIGHT;
   const float combo_marker_tw = 100.0f / HUD_TEXTURE_WIDTH;
   const float combo_marker_tx = 854.0f / HUD_TEXTURE_WIDTH;
   const float combo_marker_ty = 20.0f / HUD_TEXTURE_HEIGHT;
   const float combo_marker_w = 2 * combo_marker_tw * HUD_TEXTURE_WIDTH / w_width;
   const float combo_marker_h = combo_marker_w * (combo_marker_th * HUD_TEXTURE_HEIGHT) / (combo_marker_tw * HUD_TEXTURE_WIDTH);
   const float combo_marker_x = -1.0f;
   const float combo_marker_y = 1.0f - combo_marker_h;

   const int MAX_SPEED_DIGITS = 4;
   const int MAX_SPEED = 10000;
   const int MAX_SCORE_DIGITS = 7;
   const int MAX_COMBO_DIGITS = 10;
   const int MAX_SCORE = 10000000;
   
   static unsigned int mph_bar_ndx, mph_digit_ndx, score_digit_ndx, combo_digit_ndx;
   static float progress_bar_height;
   
   Renderer2D *renderer;
   
   std::vector<glm::vec2> posBuf, uvBuf;
   void pushDigit(int digit);
   void setDigit(int digit, int index);
   
public:
   HUD();
   
   void update(float dt, InGameState *state);
   void render(float dt);
};

#endif /* defined(__RGBZero__HUD__) */
