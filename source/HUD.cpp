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

unsigned int HUD::mph_bar_ndx, HUD::mph_digit_ndx, HUD::score_digit_ndx;
float HUD::progress_bar_height;

HUD::HUD() {
   posBuf.clear();
   uvBuf.clear();
   
   // MPH Digits
   mph_digit_ndx = posBuf.size(); 
   for (int i = 0; i < MAX_SPEED_DIGITS; i ++) {
      posBuf.push_back(glm::vec2(i *       digit_w + mph_digit_x, mph_digit_y + digit_h));
      posBuf.push_back(glm::vec2((i + 1) * digit_w + mph_digit_x, mph_digit_y));
      pushDigit(0);
   }
   
   // MPH Marker
   posBuf.push_back(glm::vec2(mph_marker_x, mph_marker_h + mph_marker_y));
   posBuf.push_back(glm::vec2(mph_marker_w + mph_marker_x, mph_marker_y));
   uvBuf.push_back(glm::vec2(mph_marker_tx, mph_marker_ty));
   uvBuf.push_back(glm::vec2(mph_marker_tx + mph_marker_tw, mph_marker_ty + mph_marker_th));

   // MPH Colored Bar
   posBuf.push_back(glm::vec2(mph_bar_x, mph_bar_h + mph_bar_y));
   uvBuf.push_back(glm::vec2(mph_bar_tx, mph_bar_ty));
   // We move the right side
   mph_bar_ndx = posBuf.size();
   posBuf.push_back(glm::vec2(mph_bar_x, mph_bar_y));
   uvBuf.push_back(glm::vec2(mph_bar_tx, mph_bar_ty + mph_bar_th));
   
   // Score Digits
   score_digit_ndx = posBuf.size();
   for (int i = 0; i < MAX_SCORE_DIGITS; i++) {
      posBuf.push_back(glm::vec2(i *       digit_w + score_digit_x, score_digit_y + digit_h));
      posBuf.push_back(glm::vec2((i + 1) * digit_w + score_digit_x, score_digit_y));
      pushDigit(0);
   }

   // Score Marker
   posBuf.push_back(glm::vec2(score_marker_x, score_marker_h + score_marker_y));
   posBuf.push_back(glm::vec2(score_marker_w + score_marker_x, score_marker_y));
   uvBuf.push_back(glm::vec2(score_marker_tx, score_marker_ty));
   uvBuf.push_back(glm::vec2(score_marker_tx + score_marker_tw, score_marker_ty + score_marker_th));

/*   // Combo Streak Marker
   posBuf.push_back(glm::vec2(combo_marker_x, combo_marker_h + combo_marker_y));
   posBuf.push_back(glm::vec2(combo_marker_x + combo_marker_h, combo_marker_y));
   uvBuf.push_back(glm::vec2(combo_marker_tx, combo_marker_ty));
   uvBuf.push_back(glm::vec2(combo_marker_tx + combo_marker_tw, combo_marker_ty + combo_marker_th));
*/   
   renderer = new Renderer2D("./textures/HUD.png");
   
   renderer->setNumElements(posBuf.size());
   renderer->bufferData(Vertices, posBuf);
   renderer->bufferData(UVs, uvBuf);
}

void HUD::setDigit(int digit, int index) {
   assert(digit >= 0 && digit <= 10);

   glm::vec2 topLeft(digit * digit_tw, digit_ty);
   glm::vec2 bottomRight = topLeft + glm::vec2(digit_tw, digit_th);
   if (index < 0) {
      uvBuf.push_back(topLeft);
      uvBuf.push_back(bottomRight);
   }
   else {
      assert(index < uvBuf.size());

      uvBuf[index]     = topLeft;
      uvBuf[index + 1] = bottomRight;
   }
}

void HUD::pushDigit(int digit) {
   setDigit(digit, -1);
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
         
         setDigit(digit, ndx);
      }
      else if (num_written) {
         setDigit(10, ndx);
      }
      else {
         setDigit(0, ndx);
      }
      
      speed = speed % digit_factor;
      digit_factor /= 10;
   }

   // MPH Bar
   float speedPercent = 0.9f * float(this->speed - PlayerPhysicsComponent::min_speed) / (PlayerPhysicsComponent::max_speed - PlayerPhysicsComponent::min_speed) + 0.1f;
   posBuf[mph_bar_ndx].x = mph_bar_x + speedPercent * mph_bar_w;
   uvBuf[mph_bar_ndx].x = mph_bar_tx + speedPercent * mph_bar_tw;
   
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

         setDigit(digit, ndx);
      }
      else if (num_written) {
         setDigit(10, ndx);
      }
      else {
         setDigit(0, ndx);
      }
      
      score = score % digit_factor;
      digit_factor /= 10;
   }
   
   renderer->bufferData(Vertices, posBuf);
   renderer->bufferData(UVs, uvBuf);
}

void HUD::render(float dt) {
   renderer->render(glm::mat4(1.0f));
}