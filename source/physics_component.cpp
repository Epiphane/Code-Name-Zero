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
   glm::vec3 new_position = obj->getPosition() + speed*dt;
   
   obj->setPosition(new_position);
}

void PlayerPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   glm::vec3 new_position = obj->getPosition();
   
   // check if ship is going to go outside of track bounds
   if (new_position.x > 3.0f) {
      new_position.x = 3.0f;
   }
   if (new_position.x < -3.0f) {
      new_position.x = -3.0f;
   }
   
   obj->setPosition(new_position);
   
   if (!DEBUG) {
//      glm::vec3 up = glm::vec3(0, 1, 0);
//      camera_setPosition(obj->getPosition() + up + glm::vec3(0, 0, 1));
//      camera_lookAt(obj->getPosition() + up);
   }
   
   
}

//void TrackPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   
//}