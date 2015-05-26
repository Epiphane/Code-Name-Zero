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
   msec_left = float(hit_time - spawn_time) / 1000; // Milliseconds between spawn and hit
   lane = spawn_lane;
}

void ObstaclePhysicsComponent::update(GameObject *obj, State *world, float dt) {
   //MovementComponent::update(obj, world, dt);
   msec_left -= dt;
   if (msec_left >= 0) {
      InGameState* game = dynamic_cast<InGameState*>(world);
      assert(game != nullptr);
      
      TrackManager* tm   = game->getTrackManager();
      GameObject *player = game->getPlayer();
      MovementComponent *movement = dynamic_cast<MovementComponent *>(player->getPhysics());
      assert(movement != nullptr);
      
      float player_long     = movement->getLongPos();
      float player_velocity = movement->getVelocity();
      
      float new_obj_long = player_long + msec_left * player_velocity / 5;
      
      glm::vec3 new_pos = tm->getPosOnTrack(new_obj_long, lane);
//      if (msec_left >= 0.2f) {
         obj->setPosition(new_pos);
//      }
   }
   
}

void MovementComponent::update(GameObject *obj, State *world, float dt) {
   velocity += accel.z * 100 * dt;
   if (velocity > 1000) velocity = 1000;
   
   // Vehicle offsets from track
   glm::vec3 longOffset = direction * long_position * TRACK_LENGTH;
   glm::vec3 latOffset = slide * lat_position * 4.0f;
   
   // Set position based on offsets
   obj->setPosition(track_position + longOffset + latOffset);
   
   // Increment longitudinal position
   long_position += dt * velocity / 10.0f;
   
   // Update lateral position if we're easing to a new place
   if (lat_position != lat_destination) {
      lat_position += (lat_destination - lat_position) / 4.0f;
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
   if (decel > 0) {
      decel -= dt;
      accel = 0;
      setAccel(getAccel() - glm::vec3(0, 0, 1));
   }
   else if (accel > 0) {
      accel -= dt;
      setAccel(getAccel() + glm::vec3(0, 0, 0.8));
   }
   
   MovementComponent::update(obj, world, dt);
}

//void TrackPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   
//}