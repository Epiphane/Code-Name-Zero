//
//  title_state.cpp
//  RGBZero
//
//  Created by David Ellison on 5/26/15.
//
//

#include "ship_select_state.h"
#include "renderer2D.h"
#include "static_state.h"
#include "in_game_state.h"
#include "input_manager.h"
#include "main.h"
#include "camera.h"

Renderer2D *background;
StaticState *bg;
int currentShip = 0;
float currentShipRotation = 0.0f;
std::vector<shared_ptr<GameObject>> ships;
std::vector<glm::mat4> carouselTransforms;

//input callbacks
void startLevel();
void modelCarouselLeft();
void modelCarouselRight();

ShipSelect::ShipSelect() {
   camera_setPosition(glm::vec3(0, 0, -5));
   camera_lookAt(glm::vec3(0, 0, 0));
   bg = new StaticState("ship_select_background", 1.5);
   background = new Renderer2D("ship_select_background", true, 0);

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

void ShipSelect::render(float dt) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   bg->render(dt);
   //background->render(glm::mat4(1));
   State::render(dt);
   
   
}

void ShipSelect::update(float dt) {
   for (int i = 0; i < MAX_SHIPS; i++) {
      ships[i]->clearModel();
   }
   
   currentShipRotation += 60.0f * dt;

   ships[currentShip]->transform(glm::rotate(glm::mat4(1.0f), currentShipRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
   
   for (int i = 0; i < MAX_SHIPS; i++) {
       float rotationDegrees = (i - currentShip) * 360/MAX_SHIPS;
      
      ships[i]->transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)));
      ships[i]->transform(glm::rotate(glm::mat4(1.0f), rotationDegrees, glm::vec3(0.0f, 1.0f, 0.0f)));
      //ships[i]->transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f)));
   }
}

void ShipSelect::start() {
   input_set_callback(GLFW_KEY_SPACE, startLevel);
   input_set_callback(GLFW_KEY_A, modelCarouselLeft);
   input_set_callback(GLFW_KEY_D, modelCarouselRight);
   
}

void ShipSelect::pause() {
   
}

void startLevel() {
   
   setState(new LoadingScreen(currentShip));
}

void modelCarouselLeft() {
   currentShipRotation = 0.0f;
   if (currentShip == 0) {
      currentShip = MAX_SHIPS - 1;
   } else {
      currentShip--;
   }
}

void modelCarouselRight() {
   currentShipRotation = 0.0f;
   if (currentShip == MAX_SHIPS - 1) {
      currentShip = 0;
   } else {
      currentShip++;
   }
}
