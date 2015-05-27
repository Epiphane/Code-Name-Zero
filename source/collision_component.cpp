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


#define ACCELERATION_AMT 1.0f
#define DECELERATION_AMT 0.8f

void PlayerCollisionComponent::collide(GameObject *player, GameObject *other) {
   ObstacleCollisionComponent *occ = dynamic_cast<ObstacleCollisionComponent *>(other->getCollision());
   
   PlayerPhysicsComponent *playerMovement = dynamic_cast<PlayerPhysicsComponent *>(player->getPhysics());
   
   if (occ->shouldAcceleratePlayer()) {
      playerMovement->accelerate(ACCELERATION_AMT);
   }
   else {
      playerMovement->decelerate(DECELERATION_AMT);
   }
}

void ObstacleCollisionComponent::collide(GameObject *thisObj, GameObject *otherObj) {
   thisObj->setCollision(nullptr);

   ObstaclePhysicsComponent *physics = dynamic_cast<ObstaclePhysicsComponent *>(thisObj->getPhysics());
   assert(physics != nullptr);

   std::cout << "Off-beat by " << physics->getTimeLeft() << " seconds." << std::endl;
}

ObstacleCollisionComponent::ObstacleCollisionComponent(Track loc, Track clr) {
   location = loc;
   color = clr;
}