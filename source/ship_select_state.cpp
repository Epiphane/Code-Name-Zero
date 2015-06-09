//
//  title_state.cpp
//  RGBZero
//
//  Created by David Ellison on 5/26/15.
//
//

#include "ship_select_state.h"
#include "static_state.h"
#include "in_game_state.h"
#include "input_manager.h"
#include "main.h"
#include "camera.h"

//input callbacks
void startLevel();
void modelCarouselLeft();
void modelCarouselRight();

ShipSelect *ShipSelect::currentInstance;
ShipSelect::ShipSelect() : StaticState("ship_select_background") {
   currentInstance = this;
   
   camera_setPosition(glm::vec3(0, 1, -10));
   camera_lookAt(glm::vec3(0, 0, 0));

   shipManager = ShipManager::instance();
   
   ships.clear();
   
   for (int i = 0; i < MAX_SHIPS; i++) {
      std::cout << shipManager->getModel(i)->getFileName() << std::endl;
      
      std::shared_ptr<GameObject> ship = std::make_shared<GameObject>(shipManager->getModel(i)->getModelRenderer());
      ships.push_back(ship);
      carouselTransforms.push_back(glm::mat4(1.0f));
      addObject(ship);
   }
   
}

void ShipSelect::update(float dt) {
   for (int i = 0; i < MAX_SHIPS; i++) {
      ships[i]->clearModel();
   }
   
   currentShipRotation += 60.0f * dt;

   ships[currentShip]->transform(glm::rotate(glm::mat4(1.0f), currentShipRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
   
   float carouselTargetRotation = currentShip * -360 / MAX_SHIPS;
   carouselRotation += (carouselTargetRotation - carouselRotation) / 4;
   for (int i = 0; i < MAX_SHIPS; i++) {
      float rotationDegrees = carouselRotation + i * 360/MAX_SHIPS;
      
      ships[i]->transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)));
      ships[i]->transform(glm::rotate(glm::mat4(1.0f), rotationDegrees, glm::vec3(0.0f, 1.0f, 0.0f)));
   }
}

void ShipSelect::start() {
   input_clear();
   
   input_set_callback(GLFW_KEY_SPACE, startLevel);
   input_set_callback(GLFW_KEY_A, modelCarouselLeft);
   input_set_callback(GLFW_KEY_D, modelCarouselRight);
   
}

void ShipSelect::rotate(bool right) {
   if (right) {
      currentShip = (currentShip + 1) % MAX_SHIPS;
      if (currentShip == 0) {
         carouselRotation += 360;
      }
   }
   else {
      currentShip = (currentShip - 1) % MAX_SHIPS;
      if (currentShip < 0) {
         currentShip += MAX_SHIPS;
         carouselRotation -= 360;
      }
   }
}

void startLevel() {
   setState(new LoadingScreen(ShipSelect::currentInstance->getCurrentShip()));
}

void modelCarouselLeft() {
   ShipSelect::currentInstance->rotate(false);
}

void modelCarouselRight() {
   ShipSelect::currentInstance->rotate(true);
}
