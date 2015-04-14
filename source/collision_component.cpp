//
//  collision_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include "collision_component.h"
#include "game_object.h"

void TargetCollisionComponent::collide(GameObject *obj, GameObject *other) {
   float dx = obj->getX() - other->getX();
   float dz = obj->getZ() - other->getZ();
   
   obj->setDirection(glm::vec3(dx, 0, dz));
}

void PlayerCollisionComponent::collide(GameObject *obj, GameObject *other) {
   other->setCollision(NULL);
   other->setPhysics(NULL);
   other->setInput(NULL);
   other->getGraphics()->getRenderer(0)->mat = MATERIAL_GRASS;
   
   score ++;
}