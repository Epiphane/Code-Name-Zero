//
//  title_state.h
//  RGBZero
//
//  Created by David Ellison on 5/26/15.
//
//

#ifndef __RGBZero__title_state__
#define __RGBZero__title_state__

#include "state.h"
#include "ship_manager.h"
#include "game_object.h"
#include "static_state.h"
#include "rendererText.h"
#include "rendererRatings.h"

class ShipSelect : public StaticState {
private:
   RendererText *helper;
   RendererRatings *ratingsRenderer;
   Renderer2D *boxRenderer;
   
   std::vector<std::string> levelNames;
   ShipManager *shipManager;
   
   int currentShip = 0;
   float carouselRotation = 0;
   float currentShipRotation = 0.0f;
   std::vector<shared_ptr<GameObject>> ships;
   std::vector<glm::mat4> carouselTransforms;
   
   void renderBox();
   void rateShipAcc(float factor, glm::vec2 start_position);
   void rateShipWeight(int factor, glm::vec2 start_position);
   
public:
   static ShipSelect *currentInstance;
   int getCurrentShip() { return currentShip; }
   void rotate(bool right);
   void clearAllRendered();
   
   ShipSelect();
   
   void start();
   void update(float dt);
   void render(float dt);
   
   std::string getLevelName();
};

#endif /* defined(__RGBZero__title_state__) */
