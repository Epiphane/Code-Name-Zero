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

void TargetCollisionComponent::collide(GameObject *obj, GameObject *other) {
   glm::vec3 diff = obj->getPosition() - other->getPosition();
   MovementComponent *movement = dynamic_cast<MovementComponent *>(obj->getPhysics());
   
   assert(movement != NULL);
   
   movement->setDirection(glm::vec3(diff.x, 0, diff.z));
}

void PlayerCollisionComponent::collide(GameObject *obj, GameObject *other) {
   MovementComponent *movement = dynamic_cast<MovementComponent *>(other->getPhysics());
   if (movement != NULL) {
      glm::vec3 direction = movement->getDirection();
      
      float angle = MATH_PI / 2;
      if (direction.z != 0) {
         angle = atan(direction.x / direction.z);
         if (direction.z <= 0)
            angle += MATH_PI;
      }
      
      other->transform(glm::rotate(angle * RADIANS_TO_DEG, glm::vec3(0, 1, 0)));
   }
   
   other->setCollision(NULL);
   other->setPhysics(NULL);
   other->setInput(NULL);
   other->getGraphics()->getRenderer(0)->mat = MATERIAL_GRASS;
   
   audio_play_sound("./audio/goal.mp3");
   
   score ++;
}