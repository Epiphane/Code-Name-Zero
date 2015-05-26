//
//  track_manager.cpp
//  Project
//
//  Created by Ryan Flatland on 4/26/15.
//
//

#include <glm/ext.hpp>

#include "track_manager.h"
#include "game_object.h"
#include "state.h"
#include "main.h"
#include "track_enum.h"
#include <math.h>


// Initialization
TrackManager::TrackManager(State *world, GameObject *player_in) {
   player = player_in;
   track_segments.clear(); // Should this not be an arrow?
   // NOTE:
   // Current track is 27.5 units long
   for (int i = 0; i<VISIBLE_TRACKS; i++) {
      GameObject *track = new GameObject(ModelRenderer::load("models/Track/track.obj", "models/Track/"));
      track->transform(nextRotate(next_track_number) * glm::scale(1.0f, 1.0f, TRACK_SCALE));
      track->setPosition(nextPosition(next_track_number));
      world->addObject(track);
      track_segments.push_back(track);
      next_track_number++;
   }
}

void TrackManager::update(float dt, glm::vec3 player_position, State *world) {
   //static int tracksSinceSpawn = 0;
   
   // Get a reference to the movement component
   MovementComponent *movement = dynamic_cast<MovementComponent *>(player->getPhysics());
   // If entering the next track segment
   while (movement->getLongPos() >= 0.5f || first) {
      first = false;
      GameObject *track = new GameObject(ModelRenderer::load("models/Track/track.obj", "models/Track/"));
      track->transform(nextRotate(next_track_number) * glm::scale(1.0f, 1.0f, TRACK_SCALE));
      track->setPosition(nextPosition(next_track_number));
      world->addObject(track);
      
      //add an object every few tracks
      /*
      if (tracksSinceSpawn++ == 25) {
         addRandomObstacle(dynamic_cast<InGameState *>(world), track->getPosition());
         tracksSinceSpawn = 0;
      }
       */
      
      track_segments.push_back(track);

      // Killing old segments 
      track_segments[0]->die();
      track_segments.erase(track_segments.begin());

      // Player movement stuff round 2
      //MovementComponent *movement = dynamic_cast<MovementComponent *>(player->getPhysics()); // Threw this one earlier...
      int curTrack = next_track_number - VISIBLE_TRACKS + 2;
      movement->setSlide(nextSlideDirection(curTrack));
      movement->setDirection(nextDirection(curTrack));
      glm::vec3 carOffset = glm::normalize(glm::cross(nextSlideDirection(curTrack),nextDirection(curTrack)));
      movement->setTrackPosition(nextPosition(curTrack) + carOffset);
      movement->setLongPos(movement->getLongPos()-1.0f);
      player->setRotation(glm::vec3(nextPitchAngle(curTrack), nextYawAngle(curTrack), nextRollAngle(curTrack)));

      next_track_number++;
   }
}

// This function defines the track with functions for the x, y, and z component of the track.  Functions must be smoothe and continuous.
glm::vec3 TrackManager::nextPosition(int track_number) {
   float curviness = 0.4f;
   float x = TRACK_LENGTH * curviness * 50.62f * cos(track_number * 0.001f) + TRACK_LENGTH * curviness * 30.7f * cos(track_number * 0.015f + 0.5f);
   float y = TRACK_LENGTH * curviness * 0.91f * sin(track_number * 0.05f) + TRACK_LENGTH * curviness * 10.8f * sin(track_number * 0.01f);
   float z = TRACK_LENGTH * -track_number; // Fine if track is mostly straight, otherwise need to actually do math
   return glm::vec3(x, y, z);
}

glm::vec3 TrackManager::nextDirection(int track_number) {
   return glm::normalize(nextPosition(track_number + 1) - nextPosition(track_number - 1));
}

float TrackManager::nextPitchAngle(int track_number) {
   glm::vec3 differential = nextPosition(track_number + 1) - nextPosition(track_number - 1);
   return atan(differential.y / differential.z) * 180.0f / MATH_PI;
}

glm::mat4 TrackManager::nextPitch(int track_number) {
   return glm::rotate(-nextPitchAngle(track_number), 1.0f, 0.0f, 0.0f);
}

float TrackManager::nextRollAngle(int track_number) {
   //glm::vec3 differential = nextPosition(track_number) - nextPosition(track_number - 1);
   //return atan(differential.x / differential.z) * 180.0f / MATH_PI;;
   return track_number * 0.0f;
}

glm::mat4 TrackManager::nextRoll(int track_number) {
   return glm::rotate(nextRollAngle(track_number), 0.0f, 0.0f, 1.0f);
}

float TrackManager::nextYawAngle(int track_number) {
   glm::vec3 differential = nextPosition(track_number + 1) - nextPosition(track_number - 1);
   return atan(differential.x / differential.z) * 180.0f / MATH_PI;
}

glm::mat4 TrackManager::nextYaw(int track_number) {
   return glm::rotate(nextYawAngle(track_number), 0.0f, 1.0f, 0.0f);
}

glm::mat4 TrackManager::nextRotate(int track_number) {
   return nextPitch(track_number) * nextYaw(track_number) * nextRoll(track_number);
}

// This method defines the right axis of the player, the one to slide along to get to a different lane
glm::vec3 TrackManager::nextSlideDirection(int track_number) {
   glm::vec3 slide = glm::vec3(cos(MATH_PI / 180.0f*nextYawAngle(track_number)), sin(MATH_PI / 180.0f*nextRollAngle(track_number)), sin(MATH_PI / 180.0f*nextYawAngle(track_number)));

   return glm::normalize(slide);
}

// internal-only helper funct
// this is so aboslutely pointless why just call rand you dont need to abstract every little thing
Track getRandomTrack() {
   int res = rand() % 3;
   return Track(res);
}

//add an obstacle with random color in one of the three Tracks
/*void TrackManager::addRandomObstacle(InGameState *world, glm::vec3 trackPos) {
   Track location = getRandomTrack();
   
   GameObject *obstacle = world->addObstacle(trackPos + glm::vec3((location - 1) * TRACK_OFFSET_RED, 1.0, 0), location, getRandomTrack());
   obstacle->transform(nextRotate(next_track_number) * glm::scale(1.5f, 1.5f, 1.5f));
}*/

//add an obstacle with defined color in a defined track on a defined beat!
void TrackManager::addObstacle(InGameState *world, glm::vec3 trackPos, int track_num, int track, int color, int obj, int spawntime, int hittime) {
   
   GameObject *obstacle = world->addObstacle(trackPos + glm::vec3((track - 1) * TRACK_OFFSET_RED, 1.0, 0), track_num, static_cast<Track>(track), static_cast<Track>(color), obj, spawntime, hittime);
   obstacle->transform(nextRotate(next_track_number) * glm::scale(1.5f, 1.5f, 1.5f));
}

// Lane input is 0,1,2
glm::vec3 TrackManager::getPosOnTrack(float longitude, int lane) {
   int track_number = next_track_number + longitude - VISIBLE_TRACKS;
   
   glm::vec3 track_center = nextPosition(track_number);
   
   float lat_offset = lane - 1;
   float long_offset = longitude - track_number;
   glm::vec3 longOffset = nextDirection(track_number) * long_offset * TRACK_LENGTH;
   glm::vec3 latOffset = nextSlideDirection(track_number) * lat_offset * 4.0f;
   
   // Set position based on offsets
   return track_center;// + longOffset + latOffset;
}
