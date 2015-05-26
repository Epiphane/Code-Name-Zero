//
//  collision_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//
#include <algorithm>
#include <glm/ext.hpp>

#include "main.h"
#include "audio_manager.h"
#include "collision_component.h"
#include "game_object.h"


#define ACCELERATION_AMT 10.0f
#define DECELERATION_AMT 8.0f

void PlayerCollisionComponent::collide(GameObject *player, GameObject *other) {
   ObstacleCollisionComponent *occ = dynamic_cast<ObstacleCollisionComponent *>(other->getCollision());
   
   MovementComponent *playerMovement = dynamic_cast<MovementComponent *>(player->getPhysics());
   
   if (!occ->hasBeenHit()) {
      if (occ->shouldAcceleratePlayer()) {
         playerMovement->setVelocity(playerMovement->getVelocity() + ACCELERATION_AMT);
         std::cout << "Accelerated" << std::endl;
      } else {
         playerMovement->setVelocity(std::max(playerMovement->getVelocity() - DECELERATION_AMT, 10.0f));
      }
   }
}

void ObstacleCollisionComponent::collide(GameObject *thisObj, GameObject *otherObj) {
   setHit(true);
}

ObstacleCollisionComponent::ObstacleCollisionComponent(Track loc, Track clr) {
   hit = false;
   location = loc;
   color = clr;
}