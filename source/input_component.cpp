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
   //Moved to physics_component.cpp : MovementComponent
}

void PlayerInputComponent::update(GameObject *obj) {
   PlayerPhysicsComponent *movement = dynamic_cast<PlayerPhysicsComponent *>(obj->getPhysics());
   if (movement != NULL) {
      glm::vec3 speed = movement->getSpeed();
      speed.x = 0.0f;
       if (!DEBUG) {
          
           if (input_keyDown(GLFW_KEY_A)) {
              speed.x -= PLAYER_SPEED;
           }
           if (input_keyDown(GLFW_KEY_D)) {
               speed.x += PLAYER_SPEED;
           }
       } else {
           if (input_keyDown(GLFW_KEY_A)) {
               camera_move(-CAMERA_MOVE, 0, 0);
           }
           if (input_keyDown(GLFW_KEY_S)) {
               camera_move(0, 0, -CAMERA_MOVE);
           }
           if (input_keyDown(GLFW_KEY_D)) {
               camera_move(CAMERA_MOVE, 0, 0);
           }
           if (input_keyDown(GLFW_KEY_W)) {
               camera_move(0, 0, CAMERA_MOVE);
           }
           if (input_keyDown(GLFW_KEY_Q)) {
               camera_move(0, CAMERA_MOVE, 0);
           }
           if (input_keyDown(GLFW_KEY_E)) {
               camera_move(0, -CAMERA_MOVE, 0);
           }
      }
      
      movement->setSpeed(speed);
   }
}