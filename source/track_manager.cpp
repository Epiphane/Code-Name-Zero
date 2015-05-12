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

#define VISIBLE_TRACKS 200


// Initialization
TrackManager::TrackManager(State *world, GameObject *player_in) {
   player = player_in;
   track_segments.clear(); // Should this not be an arrow?
   // NOTE:
   // Current track is 27.5 units long
   for (int i = 0; i<VISIBLE_TRACKS; i++) {
      GameObject *track = new GameObject(ModelRenderer::load("models/Track/RGB_TrackOnly_Curved.obj", "models/Track/"));
      track->transform(nextRotate(next_track_number) * glm::scale(1.0f, 1.0f, TRACK_SCALE));
      track->setPosition(nextPosition(next_track_number));
      world->addObject(track);
      track_segments.push_back(track);
      next_track_number++;
   }
}

void TrackManager::update(float dt, glm::vec3 player_position, State *world) {
   static int tracksSinceSpawn = 0;
   
   // Get a reference to the movement component
   MovementComponent *movement = dynamic_cast<MovementComponent *>(player->getPhysics());
   // If entering the next track segment
   if (movement->getLongPos() >= 1.0f || first) {
      first = false;
      GameObject *track = new GameObject(ModelRenderer::load("models/Track/RGB_TrackOnly_Curved.obj", "models/Track/"));
      track->transform(nextRotate(next_track_number) * glm::scale(1.0f, 1.0f, TRACK_SCALE));
      track->setPosition(nextPosition(next_track_number));
      world->addObject(track);
      
      //add an object every few tracks
      if (tracksSinceSpawn++ == 25) {
         addRandomObstacle(dynamic_cast<InGameState *>(world), track->getPosition());
         tracksSinceSpawn = 0;
      }
      
      
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
      movement->setLongPos(-1.0f);
      player->setRotation(glm::vec3(nextPitchAngle(curTrack), nextYawAngle(curTrack), nextRollAngle(curTrack)));

      next_track_number++;
   }
}

// This function defines the track with functions for the x, y, and z component of the track.  Functions must be smoothe and continuous.
glm::vec3 TrackManager::nextPosition(int track_number) {
   float curviness = 0.5;
   float x = TRACK_LENGTH * curviness * cos(track_number * 0.1f) + TRACK_LENGTH * curviness * cos(track_number * 0.15f + 0.5f);
   float y = TRACK_LENGTH * curviness * sin(track_number * 0.05f) + TRACK_LENGTH * curviness * sin(track_number * 0.1f);
   float z = TRACK_LENGTH * -track_number; // Fine if track is mostly straigh, otherwise need to actually do math;
   return glm::vec3(x,y,z);
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

//internal-only helper funct
Track getRandomTrack() {
   int res = rand() % 3;
   return Track(res);
}

//add an obstacle with random color in one of the three Tracks
void TrackManager::addRandomObstacle(InGameState *world, glm::vec3 trackPos) {
   Track location = getRandomTrack();
   
   GameObject *obstacle = world->addObstacle(trackPos + glm::vec3((location - 1) * TRACK_OFFSET_RED, 1.0, 0), location, getRandomTrack());
   obstacle->transform(nextRotate(next_track_number) * glm::scale(1.5f, 1.5f, 1.5f));
}

