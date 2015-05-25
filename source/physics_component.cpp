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

void ObstaclePhysicsComponent::init(int spawn_time, int hit_time, int spawn_lane) {
   spawn_ms = spawn_time;
   hit_ms = hit_time;
   lane = spawn_lane;
}

void ObstaclePhysicsComponent::update(GameObject *obj, State *world, float dt) {
   //MovementComponent::update(obj, world, dt);
   spawn_ms += dt;
   if (spawn_ms < hit_ms) {
      InGameState* igs = dynamic_cast<InGameState*>(world);
      float player_z = igs->getPlayer()->getPosition().z;
      float new_obj_z = float(spawn_ms)/float(hit_ms) * player_z;
      TrackManager* tm = igs->getTrackManager();
      std::cout << "Player Z = " << player_z << std::endl;
      std::cout << "track number player is on: " << tm->getTrackAtZ(player_z) << std::endl;
      std::cout << "New Z = " << new_obj_z << std::endl;
      glm::vec3 new_pos = tm->getPosOnTrack(new_obj_z, lane);
      std::cout << "New Position = " << new_pos << std::endl;
      //obj->setPosition(new_pos);
   }
   
}

void MovementComponent::update(GameObject *obj, State *world, float dt) {
   // Vehicle offsets from track
   glm::vec3 longOffset = direction * long_position * TRACK_LENGTH;
   glm::vec3 latOffset = slide * lat_position * 4.0f;

   // Set position based on offsets
   obj->setPosition(track_position + longOffset + latOffset);

   // Increment longitudinal position
   long_position += dt * velocity;

   /* Previous physics
   float sp = glm::length(speed) + accel.z;
   glm::vec4 new_speed = glm::vec4(speed, 0);
   new_speed = glm::rotate(accel.y * dt, 1.0f, 0.0f, 0.0f) * new_speed;
   new_speed = glm::rotate(accel.x * dt, 0.0f, -1.0f, 0.0f) * new_speed;
   speed = glm::normalize(glm::vec3(new_speed));
   speed *= sp;
   
//   speed += accel * dt;
   
   glm::vec3 new_position = obj->getPosition() + speed * dt;
   
   obj->setPosition(new_position);
   
//   accel = glm::vec3(0);
*/
}

void MovementComponent::changeLatPos(float a) {
   float max = 1.0;
   float step = 0.1;
   if (a < 0) {
      if (lat_position <= -max) {
         lat_position = -max;
      }
      else
         lat_position -= step;
   }
   if (a > 0) {
      if (lat_position >= max) {
         lat_position = max;
      }
      else
         lat_position += step;
   }
}

void PlayerPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   glm::vec3 new_position = obj->getPosition();
   glm::vec3 new_speed = getSpeed();
   
   // check if ship is going to go outside of track bounds
//   if (new_position.x > 3.0f) {
//      new_position.x = 3.0f;
//   }
//   if (new_position.x < -3.0f) {
//      new_position.x = -3.0f;
//   }
   if (new_position.y < 0.0f) {
      new_position.y = 0.0f;
      new_speed.y = 0;
   }
   
//   float SPEED_LIMIT = 20;
//   if (new_speed.x < -SPEED_LIMIT) {
//      new_speed.x = -SPEED_LIMIT;
//   }
//   if (new_speed.x > SPEED_LIMIT) {
//      new_speed.x = SPEED_LIMIT;
//   }
//   if (new_speed.y < -SPEED_LIMIT) {
//      new_speed.y = -SPEED_LIMIT;
//   }
//   if (new_speed.y > SPEED_LIMIT) {
//      new_speed.y = SPEED_LIMIT;
//   }
   
   setSpeed(new_speed);
   //obj->setPosition(new_position);
}

//void TrackPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   
//}