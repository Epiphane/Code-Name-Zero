//
//  collision_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include <glm/ext.hpp>

#include "main.h"
#include "audio_manager.h"
#include "collision_component.h"
#include "game_object.h"

void PlayerCollisionComponent::collide(GameObject *obj, GameObject *other) {
   
}

void ObstacleCollisionComponent::collide(GameObject *thisObj, GameObject *otherObj) {
   //if other object is a player object (if its CollisionComponent is a PlayerCollisionComponent)
   //then set this as hit
   if (otherObj->getCollision() != nullptr) {
      if (dynamic_cast<PlayerCollisionComponent *>(otherObj->getCollision())) {
         printf("I'm an ObjectCollisionComponent and I just collided with a player\n");
      }
   }
}