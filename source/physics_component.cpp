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
#include "in_game_state.h"
#include "track_manager.h"

void ObstaclePhysicsComponent::init(float travel_time) {
   sec_left = travel_time; // Milliseconds between spawn and hit
}

float CALIBRATION = 0.2f;
void ObstaclePhysicsComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   sec_left -= dt;
   //if (sec_left >= 0) {
      InGameState* game = dynamic_cast<InGameState*>(world);
      assert(game != nullptr);

      float player_speed = game->getPlayerSpeed();
      glm::vec3 new_pos = obj->getPosition();
      new_pos.z = (sec_left + CALIBRATION) * -player_speed;

      //if (sec_left > 0.2f) {
      obj->setPosition(new_pos);
      //}
   //}
   
}

void MovementComponent::update(GameObject *obj, State *world, float dt) {
   InGameState *game = dynamic_cast<InGameState *>(world);
   if (game != nullptr) {
      float speed = game->getPlayerSpeed();

      obj->setPosition(obj->getPosition() + glm::vec3(0, 0, dt * speed));
   }
}

void PlayerPhysicsComponent::update(GameObject *obj, State *world, float dt) {
   if (accel_time > 0) {
      speed += accel * dt * 30;
      accel_time -= dt;
   }
   else {
      accel = 0;
   }

   if (speed < min_speed) speed = min_speed;
   if (speed > max_speed) speed = max_speed;

   glm::vec3 pos = obj->getPosition();
   pos.x += (lat_destination - pos.x) / 4;
   obj->setPosition(pos);
   
   // Tint the player
   Track current_track = GREEN;
   if (pos.x < TRACK_OFFSET_BLUE / 2)
      current_track = BLUE;
   else if (pos.x > TRACK_OFFSET_RED / 2)
      current_track = RED;
   
   if (obj->getGraphics() != nullptr)
      obj->getGraphics()->tint(current_track);
}

void PlayerPhysicsComponent::accelerate(float time, float _accel) {
   accel = _accel;
   accel_time = time;
}

void PlayerPhysicsComponent::decelerate(float time, float _decel) {
   accelerate(time, -_decel);
}