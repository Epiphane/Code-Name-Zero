//
//  physics_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include <glm/ext.hpp>

#include "main.h"
#include "camera.h"
#include "physics_component.h"
#include "game_object.h"
#include "state.h"

void MovementComponent::update(GameObject *obj, State *world, float dt) {
   float lat_speed = latSpeed * dt;
   float world_speed = speed * dt;
   
   //Check for the edge of the platforms
   glm::vec3 pos = obj->getPosition();
   
   if (obj->getType() != OBJECT_PLAYER && (fabs(pos.x) > GROUND_WIDTH/2 || fabs(pos.z) > GROUND_WIDTH/2)) {
      //Find a new direction for the object
      direction = glm::normalize(randPoint(GROUND_WIDTH/3) - glm::vec3(pos.x, 0, pos.z));
   } else if (fabs(pos.x) > GROUND_WIDTH/2 || fabs(pos.z) > GROUND_WIDTH/2) {
      //pos.x = 0;
      //pos.z = 0;
   }
   
   glm::vec3 crossed = glm::cross(direction, glm::vec3(0, 1, 0));
   if (obj->getType() != OBJECT_PLAYER)
       obj->transform(glm::rotate(this->getSpeed() * dt * FRAMES_PER_SEC / 5, 1.0f, 0.0f, 0.0f));
   pos += world_speed * direction + lat_speed * crossed;
   if (pos.x > 3.0f){
      pos.x = 3.0f;
      obj->setPosition(pos);
   }
   else if(pos.x < -3.0f) {
      pos.x = -3.0f;
      obj->setPosition(pos);
   }
   else {
      obj->setPosition(pos);
   }
}

void PlayerMovementComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   if (!DEBUG)
       camera_setPosition(obj->getPosition() + glm::vec3(0, 1, 0));
}

void TrackPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   obj->setPosition(obj->getPosition() + glm::vec3(0, 0, 100 * dt));
   
   if (obj->getPosition().z > 30.0f) {
      obj->die();
   }
}