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
   float lat_speed = latSpeed / FRAMES_PER_SEC;
   float world_speed = speed / FRAMES_PER_SEC;
   
   //Check for the edge of the platforms
   glm::vec3 pos = obj->getPosition();
   
   if (obj->getType() != OBJECT_PLAYER && (fabs(pos.x) > GROUND_WIDTH/2 || fabs(pos.z) > GROUND_WIDTH/2)) {
      //Find a new direction for the object
      direction = glm::normalize(randPoint(GROUND_WIDTH/3) - glm::vec3(pos.x, 0, pos.z));
   } else if (fabs(pos.x) > GROUND_WIDTH/2 || fabs(pos.z) > GROUND_WIDTH/2) {
      pos.x = 0;
      pos.z = 0;
   }
   
   glm::vec3 crossed = glm::cross(direction, glm::vec3(0, 1, 0));
   
   pos += world_speed * direction + lat_speed * crossed;
   obj->setPosition(pos);
}

void PlayerMovementComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   camera_setPosition(obj->getPosition() + glm::vec3(0, 1, 0));
}