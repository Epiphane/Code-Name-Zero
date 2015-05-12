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


#define ACCELERATION_AMT 0.4f

void PlayerCollisionComponent::collide(GameObject *player, GameObject *other) {
   ObstacleCollisionComponent *occ = dynamic_cast<ObstacleCollisionComponent *>(other->getCollision());
   
   PlayerPhysicsComponent *playerMovement = dynamic_cast<PlayerPhysicsComponent *>(player->getPhysics());
   
   if (!occ->hasBeenHit()) {
      if (occ->shouldAcceleratePlayer()) {
         playerMovement->accelerate(ACCELERATION_AMT);
         printf("Accelerated!");
      } else {
         playerMovement->decelerate(ACCELERATION_AMT);
         printf("Decelerated.");
      }
      printf(" New velocity: %f\n", playerMovement->getVelocity());
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