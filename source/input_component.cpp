//
//  input_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include "main.h"
#include "camera.h"
#include "input_component.h"
#include "physics_component.h"
#include "game_object.h"

const float PLAYER_SPEED = 20;

void WheelInputComponent::update(GameObject *obj) {
   obj->rotation.z += obj->getSpeed() / 5;
}

void PlayerInputComponent::update(GameObject *obj) {
   obj->setDirection(camera_getLookAt());
   float speed = 0;
   float latSpeed = 0;
   
   if (keysDown[GLFW_KEY_A]) {
      latSpeed -= PLAYER_SPEED;
   }
   if (keysDown[GLFW_KEY_S]) {
      speed -= PLAYER_SPEED;
   }
   if (keysDown[GLFW_KEY_D]) {
      latSpeed += PLAYER_SPEED;
   }
   if (keysDown[GLFW_KEY_W]) {
      speed += PLAYER_SPEED;
   }
   
   obj->setSpeed(speed);
   obj->setLatSpeed(latSpeed);
   
   camera_setPosition(glm::vec3(obj->getX(), obj->getY() + 2, obj->getZ()));
}