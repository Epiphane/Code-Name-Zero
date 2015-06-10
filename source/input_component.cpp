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

void PlayerInputComponent::update(GameObject *obj) {
   PlayerPhysicsComponent *movement = dynamic_cast<PlayerPhysicsComponent *>(obj->getPhysics());
   if (movement != NULL) {
      if (!PAUSED) {
         Track currentTrack = getTrackFromLatPos(obj->getPosition().x);
         if ((input_keyDown(GLFW_KEY_LEFT) || input_keyDown(GLFW_KEY_A)) && currentTrack != BLUE) {
            int nextLatPos = (currentTrack == GREEN) ? -1 : 0;
            movement->setLatPos(nextLatPos);
         }
         if ((input_keyDown(GLFW_KEY_RIGHT) || input_keyDown(GLFW_KEY_D)) && currentTrack != RED) {
            int nextLatPos = (currentTrack == GREEN) ? 1 : 0;
            movement->setLatPos(nextLatPos);
         }
         if (input_keyDown(GLFW_KEY_Z)) {
            movement->setLatPos(-1);
         }
         if (input_keyDown(GLFW_KEY_X) || input_keyDown(GLFW_KEY_S)) {
            movement->setLatPos(0);
         }
         if (input_keyDown(GLFW_KEY_C)) {
            movement->setLatPos(1);
         }
      }
   }
}