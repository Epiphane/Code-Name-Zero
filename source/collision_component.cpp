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


#define ACCELERATION_AMT 0.75f
#define DECELERATION_AMT 0.75f

void PlayerCollisionComponent::collide(GameObject *player, GameObject *other) {
   ObstacleCollisionComponent *occ = dynamic_cast<ObstacleCollisionComponent *>(other->getCollision());
   
   PlayerPhysicsComponent *playerMovement = dynamic_cast<PlayerPhysicsComponent *>(player->getPhysics());
   
   if (occ->shouldAcceleratePlayer(getTrackFromLatPos(playerMovement->getLatPos()))) {
      playerMovement->accelerate(ACCELERATION_AMT);
   }
   else {
      playerMovement->decelerate(DECELERATION_AMT);
   }
}

void ObstacleCollisionComponent::collide(GameObject *thisObj, GameObject *otherObj) {
   thisObj->setCollision(nullptr);
}

ObstacleCollisionComponent::ObstacleCollisionComponent(Track loc, Track clr, ObstacleType obsType) {
   location = loc;
   color = clr;
   type = obsType;
}

bool ObstacleCollisionComponent::shouldAcceleratePlayer(Track playerLane) {
   if (type == WALL) {
      return color == playerLane;
   } else {
      return color == location;
   }
}
