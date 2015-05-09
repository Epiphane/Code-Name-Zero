//
//  track_manager.h
//  Project
//
//  Created by Ryan Flatland on 4/26/15.
//
//

#ifndef __Project__track__
#define __Project__track__

#include <vector>
#include <glm/ext.hpp>

#include "game_object.h"

#define TRACK_SCALE 0.2f
#define TRACK_LENGTH 27.5f * TRACK_SCALE

class TrackManager {
private:
   std::vector<GameObject*> track_segments;
   GameObject* player;
   // Farthest Z value of track
   int next_track_number = 0;
   glm::vec3 slide_direction;
   bool first = true;

public:
   TrackManager(State *world, GameObject *player_in);
   void update(float dt, glm::vec3 player_position, State *world);
   glm::vec3 nextPosition(int track_number);
   glm::vec3 nextDirection(int track_number);

   float nextPitchAngle(int track_number);
   glm::mat4 nextPitch(int track_number);
   float nextRollAngle(int track_number);
   glm::mat4 nextRoll(int track_number);
   float nextYawAngle(int track_number);
   glm::mat4 nextYaw(int track_number);
   

   glm::mat4 nextRotate(int track_number);
   glm::vec3 nextSlideDirection(int track_number);

   glm::vec3 getSlideDirection() { return slide_direction; };
};

#endif /* defined(__Project__track__) */