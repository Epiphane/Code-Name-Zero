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
#include "in_game_state.h"

#define TRACK_SCALE 0.2f
#define TRACK_LENGTH 27.5f * TRACK_SCALE
#define TRACK_OBJECT "models/Track/RGB_TrackOnly_Curved.obj"

#define TRACK_OFFSET_BLUE -3.8
#define TRACK_OFFSET_GREEN 0
#define TRACK_OFFSET_RED 3.8

#define VISIBLE_TRACKS 200

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
   std::vector<GameObject*> getSegments(){return track_segments;};

   glm::vec3 getSlideDirection() { return slide_direction; };
   
   //add a random obstacle based on the position of a track
   void addRandomObstacle(InGameState *world, glm::vec3 trackPos);
   void addObstacle(InGameState *world, glm::vec3 trackPos, int track_num, int track, int color, int obj, int spawntime, int hittime);
   int nextTrack_number() {return next_track_number;};
   glm::vec3 getPosOnTrack(float longitude, int lane);
};

#endif /* defined(__Project__track__) */