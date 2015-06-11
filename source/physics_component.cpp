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
#include "audio_manager.h"
#include "physics_component.h"
#include "game_object.h"
#include "in_game_state.h"
#include "track_manager.h"

void ObstaclePhysicsComponent::init(float travel_time) {
   sec_left = travel_time; // Milliseconds between spawn and hit
}

float CALIBRATION = 0.1f;
void ObstaclePhysicsComponent::update(GameObject *obj, State *world, float dt) {
   MovementComponent::update(obj, world, dt);
   
   sec_left -= dt;
   InGameState* game = dynamic_cast<InGameState*>(world);
   assert(game != nullptr);

   float player_speed = game->getPlayerSpeed();
   glm::vec3 new_pos = obj->getPosition();
   new_pos.z = (sec_left + CALIBRATION) * -player_speed;

   obj->setPosition(new_pos);

   // Get rid of dead objects
   if (new_pos.z > 1)
      obj->die();
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

   float lean = -4.0f * (lat_destination - pos.x);
   obj->setRotation(glm::vec3(0.0f, 0.0f, lean)); // Add banking
   
   // Tint the player
   Track current_track = GREEN;
   if (pos.x < TRACK_OFFSET_BLUE / 2)
      current_track = BLUE;
   else if (pos.x > TRACK_OFFSET_RED / 2)
      current_track = RED;
   
   if (obj->getGraphics() != nullptr) {
      obj->getGraphics()->tint(current_track);
      obj->getGraphics()->setPowerup((accel_time + dt) * accel);
   }
}

void PlayerPhysicsComponent::accelerate(ShipModel *playerShip, float time, float _accel) {
	if (_accel > 0) {
		if (++comboStreak >= playerShip->getStreakNumber()) {
			_accel *= playerShip->getBoostEffect();
			resetComboStreak();

         audio_play_sound("./audio/sfx/hit8.mp3");
		}
      else {
         int hit = comboStreak * 8 / playerShip->getStreakNumber();
         if (hit <= 0) {
            hit = 1;
         }

         audio_play_sound("./audio/sfx/hit" + std::to_string(hit) + ".mp3");
      }
	}
	else {
		resetComboStreak();
	}
   accel = _accel;
   accel_time = time;
}

void PlayerPhysicsComponent::decelerate(ShipModel *playerShip, float time, float _decel) {
   accelerate(playerShip, time, -_decel);
}