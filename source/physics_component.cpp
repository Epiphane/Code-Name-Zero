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
#include "track_manager.h"

void MovementComponent::update(GameObject *obj, State *world, float dt) {
   // Vehicle offsets from track
   glm::vec3 longOffset = direction * long_position * TRACK_LENGTH;
   glm::vec3 latOffset = slide * lat_position * 4.0f;

   // Set position based on offsets
   obj->setPosition(track_position + longOffset + latOffset);

   // Increment longitudinal position
   long_position += dt * velocity;

   // Update lateral position if we're easing to a new place
   if (lat_position != lat_destination) {
      lat_position += (lat_destination - lat_position) / 10.0f;
   }
}

void MovementComponent::setLatPos(float pos, bool instant) {
   // Refine bounds
   if (pos < -1.0f)
      pos = -1.0f;
   else if (pos > 1.0f)
      pos = 1.0f;

   if (instant) {
      lat_position = lat_destination = pos;
   }
   else {
      lat_destination = pos;
   }
}

void PlayerPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   glm::vec3 new_position = obj->getPosition();
   glm::vec3 new_speed = getSpeed();
   
   // check if ship is going to go outside of track bounds
   if (new_position.y < 0.0f) {
      new_position.y = 0.0f;
      new_speed.y = 0;
   }
   
   setSpeed(new_speed);
}