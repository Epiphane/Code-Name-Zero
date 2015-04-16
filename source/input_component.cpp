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
   MovementComponent *movement = dynamic_cast<MovementComponent *>(obj->getPhysics());
   if (movement != NULL) {
      float speed = 0;
      float latSpeed = 0;

       if (!DEBUG) {
           movement->setDirection(camera_getLookAt());
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
      movement->setLatSpeed(latSpeed);
   }
}