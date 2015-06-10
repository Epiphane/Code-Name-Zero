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
#include "in_game_state.h"
#include "main.h"
#include "track_enum.h"
#include <math.h>

// Static variables
GraphicsComponent *TrackManager::renderer;

TrackManager::TrackManager() : next_track_number(0), zpos(0), song_progress(0) {
   // Load track OBJ
   renderer = ModelRenderer::load(TRACK_OBJECT, "models/Track/");

   tracks.reserve(VISIBLE_TRACKS);
   tracks.clear();
   for (int i = 0; i < VISIBLE_TRACKS; i++) {
      addTrack();
   }
}

const glm::mat4 TrackManager::Z_TRANSLATE(glm::translate(0.0f, 0.0f, -TRACK_LENGTH));
glm::mat4 TrackManager::getTransform(int track_number) {
   glm::mat4 transform = glm::mat4(1);
   float angle = nextTrackAngle(track_number);
   transform = glm::rotate(angle, 0.0f, 1.0f, 0.0f);

   // Always move the piece backwards too!
   return Z_TRANSLATE * transform;
}

float TrackManager::nextTrackAngle(int track_number) {
   float angle = 0;
   float x = song_progress;
   float magnitude = mag[0] * pow(x, 6) + mag[1] * pow(x, 5) + mag[2] * pow(x, 4) + mag[3] * pow(x, 3) + mag[4] * pow(x, 2) + mag[5] * x + mag[6];
   float freq = frq[0] * pow(x, 6) + frq[1] * pow(x, 5) + frq[2] * pow(x, 4) + frq[3] * pow(x, 3) + frq[4] * pow(x, 2) + frq[5] * x + frq[6];
   if (song_progress > curveBegin) // Right after intro
      angle = magnitude * (sinf(freq * (track_number / 10.0f)) + 0.73 * cosf(freq * (track_number / 10.0f)));
   return angle;
}

glm::mat4 TrackManager::translate(glm::vec3 pos) {
   // First, get the track piece it's on top of
   int track_num = (pos.z - zpos) / -TRACK_LENGTH;

   if (track_num >= tracks.size()) {
      // Off the track! Clip object
      return glm::mat4(0);
   }

   glm::mat4 transform = tracks[track_num];

   if (track_num < 1) {
      return glm::translate(pos);
   }
   else {
      transform = second_track * transform;
   }
   
   glm::vec4 base = transform * glm::vec4(0, 0, 0, 1);
   glm::mat4 offset = glm::translate(pos.x, 0.0f, pos.z - base.z);

   return transform * offset;
}

void TrackManager::addTrack() {
   glm::mat4 transform = getTransform(next_track_number++);
   if (tracks.size() <= 1) {
      // Don't transform track #1 or #2 from previous tracks
      tracks.push_back(transform);
   }
   else {
      // Stack transformations
      tracks.push_back(tracks[tracks.size() - 1] * transform);
   }
}

void TrackManager::update(float dt, State *world) {
   INIT_BENCHMARK;
   InGameState *game = dynamic_cast<InGameState *>(world);
   if (game != nullptr) {
      song_progress = game->getSoundtrack()->getProgress();
      
      // Move the track under the player
      float speed = game->getPlayerSpeed();
      zpos += speed * dt;

      // Get rid of old tracks
      glm::mat4 deleted_transform = glm::mat4(1);
      int       deleted_tracks = 0;
      while (zpos > TRACK_LENGTH) {
         deleted_tracks++;

         // Fix the z position
         zpos -= TRACK_LENGTH;

         // Since tracks[0] is "special" we don't
         // un-transform it from anything.
         deleted_transform = tracks[1];

         // If this is our last iteration of the while loop,
         // Make sure that tracks[0] is the simplest transform
         // possible. i.e. If you have [A, B, B*C, B*C*D, ...] and
         // shift two track pieces, you want [C, D, D*E, ...],
         // NOT [B*C, D, D*E, ...
         if (zpos <= TRACK_LENGTH && deleted_tracks > 1) {
            tracks[1] *= glm::inverse(tracks[0]);
         }

         tracks.erase(tracks.begin());
      }

      // Compensate for missing tracks
      if (deleted_tracks > 0) {
         glm::mat4 reverse_transform = glm::inverse(deleted_transform);

         // Reverse the transformation
         int ndx = 1;
         for (; ndx < tracks.size(); ndx++) {
            tracks[ndx] = reverse_transform * tracks[ndx];
         }

         // And replace the deleted tracks
         for (; ndx < VISIBLE_TRACKS; ndx++)
            addTrack();
      }
   }

   // Update first and second tracks' transformation matrices
   first_track = glm::translate(0.0f, 0.0f, zpos);
   renderer->render(first_track);

   // How much do we un-transform the second piece?
   float dtransform = zpos / TRACK_LENGTH;
   second_track = tracks[0] * (1 - dtransform) + Z_TRANSLATE * dtransform;
   second_track = first_track * second_track; // Move it with the first piece

   COMPUTE_BENCHMARK(25, "Track update: ", true)
}

const Bounds track_bounds = {
   0, 1, 0, 1, 0, TRACK_LENGTH
};

void TrackManager::render() {
   INIT_BENCHMARK
      renderer->render(first_track);
   renderer->render(second_track);

   // Iterator for all track pieces
   std::vector<glm::mat4>::iterator transform = tracks.begin();
   transform++; // Move onto the third piece (tracks[1])

   // Now, draw the rest as usual
   while (transform < tracks.end()) {
      renderer->render(second_track * (*transform));
      RendererDebug::instance()->renderBounds(glm::vec3(second_track * (*transform) * glm::vec4(0, 0, 0, 1)), track_bounds);
      transform++;
   }

   COMPUTE_BENCHMARK(25, "Track render: ", true)
}

void TrackManager::initCurveEq(float startCurves,
   float mag1, float mag2, float mag3, float mag4, float mag5, float mag6, float mag7,
   float frq1, float frq2, float frq3, float frq4, float frq5, float frq6, float frq7) {

   curveBegin = startCurves;

   mag[0] = mag1;
   mag[1] = mag2;
   mag[2] = mag3;
   mag[3] = mag4;
   mag[4] = mag5;
   mag[5] = mag6;
   mag[6] = mag7;

   frq[0] = frq1;
   frq[1] = frq2;
   frq[2] = frq3;
   frq[3] = frq4;
   frq[4] = frq5;
   frq[5] = frq6;
   frq[6] = frq7;
}