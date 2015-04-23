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

void PlayerPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   glm::vec3 new_position = obj->getPosition() + speed*dt;
   
   // check if ship is going to go outside of track bounds
   if (new_position.x > 3.0f) {
      new_position.x = 3.0f;
   }
   if (new_position.x < -3.0f) {
      new_position.x = -3.0f;
   }
   
   obj->setPosition(new_position);
   
   if (!DEBUG) {
      camera_setPosition(obj->getPosition() + glm::vec3(0, 1, 0));
      camera_lookAt(obj->getPosition() + glm::vec3(0, 1, -2));
   }
   
   
}

//void TrackPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   
//}