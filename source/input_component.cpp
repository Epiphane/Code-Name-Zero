//
//  input_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include <glm/ext.hpp>

#include "main.h"
#include "camera.h"
#include "input_component.h"
#include "physics_component.h"
#include "game_object.h"

const float PLAYER_SPEED = 20;

void WheelInputComponent::update(GameObject *obj) {
   MovementComponent *movement = dynamic_cast<MovementComponent *>(obj->getPhysics());
   if (movement != NULL) {
      obj->transform(glm::rotate(movement->getSpeed() / 5, 1.0f, 0.0f, 0.0f));
   }
}

void PlayerInputComponent::update(GameObject *obj) {
   MovementComponent *movement = dynamic_cast<MovementComponent *>(obj->getPhysics());
   if (movement != NULL) {
      movement->setDirection(camera_getLookAt());
      float speed = 0;
      float latSpeed = 0;
      
      if (input_keyDown(GLFW_KEY_A)) {
         latSpeed -= PLAYER_SPEED;
      }
      if (input_keyDown(GLFW_KEY_S)) {
         speed -= PLAYER_SPEED;
      }
      if (input_keyDown(GLFW_KEY_D)) {
         latSpeed += PLAYER_SPEED;
      }
      if (input_keyDown(GLFW_KEY_W)) {
         speed += PLAYER_SPEED;
      }
      
      movement->setSpeed(speed);
      movement->setLatSpeed(latSpeed);
      
      camera_setPosition(obj->getPosition() + glm::vec3(0, 2, 0));
   }
}