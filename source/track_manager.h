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

#include "graphics_component.h"

#define TRACK_LENGTH 27.5f
#define TRACK_OBJECT "models/Track/RGB_TrackOnly_Curved.obj"

#define TRACK_OFFSET_BLUE -3.8
#define TRACK_OFFSET_GREEN 0
#define TRACK_OFFSET_RED 3.8

#define VISIBLE_TRACKS 200

class State;

class TrackManager {
private:
   const static glm::mat4 Z_TRANSLATE;

   // Track model
   static ModelRenderer *renderer;

   // State of the track
   int next_track_number;
   float zpos, song_progress;

   // Keeps "track" of all active tracks
   glm::mat4 first_track, second_track;
   std::vector<glm::mat4> tracks;
   void addTrack();

public:
   TrackManager();

   void setTrackNumber(int track_number) { next_track_number = track_number; }

   glm::mat4 getTransform(int track_num);
   glm::mat4 translate(glm::vec3 position);
   
   void update(float dt, State *world);
   void render();
};

#endif /* defined(__Project__track__) */