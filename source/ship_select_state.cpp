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

ShipModel *currentShipModel;

ShipSelect *ShipSelect::currentInstance;
ShipSelect::ShipSelect() : StaticState("ship_select_background") {
   currentInstance = this;
   
   helper = RendererText::instance();
   boxRenderer = new Renderer2D("./textures/darkbluebg.png", true, 0.5f);
   renderBox();
   
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

void ShipSelect::renderBox() {
   std::vector<glm::vec2> positions, uvs;
   positions.push_back(glm::vec2(0.30, 0.9));
   positions.push_back(glm::vec2(0.93, 0.2));
   uvs.push_back(glm::vec2(0));
   uvs.push_back(glm::vec2(1));
   
   std::vector<float> opacities;
   opacities.push_back(1);
   opacities.push_back(1);
   
   boxRenderer->bufferData(Opacities, opacities);
   boxRenderer->bufferData(Vertices, positions);
   boxRenderer->bufferData(UVs, uvs);
}

void ShipSelect::update(float dt) {
   for (int i = 0; i < MAX_SHIPS; i++) {
      ships[i]->clearModel();
   }
   
   currentShipRotation += 60.0f * dt;

   ships[currentShip]->transform(glm::rotate(glm::mat4(1.0f), currentShipRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
   currentShipModel = shipManager->getModel(currentShip);
   
   float carouselTargetRotation = currentShip * -360 / MAX_SHIPS;
   carouselRotation += (carouselTargetRotation - carouselRotation) / 4;
   for (int i = 0; i < MAX_SHIPS; i++) {
      float rotationDegrees = carouselRotation + i * 360/MAX_SHIPS;
      
      ships[i]->transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)));
      ships[i]->transform(glm::rotate(glm::mat4(1.0f), rotationDegrees, glm::vec3(0.0f, 1.0f, 0.0f)));
   }
   
   //display ship information
   helper->clearAllText();
   helper->addText(glm::vec2(0.6, 0.8), currentShipModel->getFileName(), glm::vec2(0.075), 1.0f);
   helper->addText(glm::vec2(0.6, 0.70), "Maker- " + currentShipModel->getMakerName(), glm::vec2(0.04), 1.0f);
   helper->addText(glm::vec2(0.6, 0.65), "Engine- " + currentShipModel->getEngineName(), glm::vec2(0.04), 1.0f);
   helper->addText(glm::vec2(0.6, 0.60), "Weight- " + currentShipModel->getWeight(), glm::vec2(0.04), 1.0f);
   helper->addText(glm::vec2(0.6, 0.55), "Acceleration- " + std::to_string((int) (currentShipModel->getAccFactor() * 100)) + " Percent", glm::vec2(0.04), 1.0f);
   
   helper->updateBuffers();
}

void ShipSelect::render(float dt) {
   this->StaticState::render(dt);
   helper->render();
   boxRenderer->render(glm::mat4(1.0f));
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

void ShipSelect::clearText() {
   helper->clearAllText();
}

void startLevel() {
   ShipSelect::currentInstance->clearText();
   setState(new LoadingScreen(ShipSelect::currentInstance->getCurrentShip()));
}

void modelCarouselLeft() {
   ShipSelect::currentInstance->rotate(false);
}

void modelCarouselRight() {
   ShipSelect::currentInstance->rotate(true);
}
