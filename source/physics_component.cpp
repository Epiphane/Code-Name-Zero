//
//  physics_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include "main.h"
#include "camera.h"
#include "physics_component.h"
#include "game_object.h"
#include "state.h"

void MovementComponent::update(GameObject *obj, State *world, float dt) {
   float lat_speed = obj->getLatSpeed() / FRAMES_PER_SEC;
   float world_speed = obj->getSpeed() / FRAMES_PER_SEC;
   
   //Check for the edge of the platforms
   if (obj->type != OBJECT_PLAYER && (fabs(obj->getX()) > GROUND_WIDTH/2 || fabs(obj->getZ()) > GROUND_WIDTH/2)) {
      //Find a new direction for the object
      glm::vec3 newDirectionPoint = randPoint(GROUND_WIDTH/3);
      obj->setDirection(newDirectionPoint - glm::vec3(obj->getX(), 0, obj->getZ()));
   } else if (fabs(obj->getX()) > GROUND_WIDTH/2 || fabs(obj->getZ()) > GROUND_WIDTH/2) {
      obj->setZ(0);
      obj->setX(0);
   }
   
   obj->setZ(obj->getZ() + world_speed * obj->getDirection().z);
   obj->setX(obj->getX() + world_speed * obj->getDirection().x);
   
   glm::vec3 crossed = glm::cross(obj->getDirection(), glm::vec3(0, 1, 0));
   obj->setZ(obj->getZ() + lat_speed * crossed.z);
   obj->setX(obj->getX() + lat_speed * crossed.x);
}

void PlayerMovementComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   camera_setPosition(glm::vec3(obj->getX(), obj->getY() + 1, obj->getZ()));
}