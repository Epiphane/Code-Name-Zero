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

const float PLAYER_SPEED = 100;

void WheelInputComponent::update(GameObject *obj) {
   //Moved to physics_component.cpp : MovementComponent
}

void PlayerInputComponent::update(GameObject *obj) {
   MovementComponent *movement = dynamic_cast<MovementComponent *>(obj->getPhysics());
   if (movement != NULL) {
      glm::vec3 accel = movement->getSpeed();
      accel.x = accel.y = 0.0f;
      accel = glm::vec3(0);
      if (!DEBUG) {
         if (input_keyDown(GLFW_KEY_W)) {
            accel.y -= PLAYER_SPEED;
         }
         if (input_keyDown(GLFW_KEY_S)) {
            accel.y += PLAYER_SPEED;
         }
         if (input_keyDown(GLFW_KEY_A)) {
            accel.x -= PLAYER_SPEED;
         }
         if (input_keyDown(GLFW_KEY_D)) {
            accel.x += PLAYER_SPEED;
         }
         if (input_keyDown(GLFW_KEY_Q)) {
            accel.z -= 1;
         }
         if (input_keyDown(GLFW_KEY_E)) {
            accel.z += 1;
         }
      }
      
      movement->setAccel(accel);
   }
}