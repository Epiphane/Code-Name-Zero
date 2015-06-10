//
//  beatEventListener.cpp
//  RGBZero
//
//  Created by Max Linsenbard on 5/18/15.
//
//
#include <fstream>
#include <random>
#include <sys/stat.h>
#include <math.h>
#include "beatEventListener.h"
#include "main.h"
#include "audio_manager.h"
#include "in_game_state.h"
#include "track_manager.h"
#include "track_enum.h"
#include "state.h"

BeatEventListener::BeatEventListener() {
   
}

BeatEventListener::~BeatEventListener() {
   events.clear();
}

/******* BEAT FILE FORMAT *********/
/*-------------------*/
/* LINE | CONTENT    */
/*-------------------*/
// 1    | SONG NAME  //
/*-------------------*/
// 2    | SONG BPM   //
/*-------------------*/
// 3    | TRACK VALS //
/*-------------------*/
// 4+   | BEATMAP    //
/******** END FILE ****************/

void BeatEventListener::init(std::string filename, State *world) {
   struct stat buffer;
   int beat = 0;
   int obj = 0;
   int lane = 0;
   int color = 0;
   lastBeat = 0;
   
   InGameState* s = dynamic_cast<InGameState *>(world);
   assert(s != nullptr);
   Music* m = s->getSoundtrack();
   int bpm = m->getBPM();
   int totalBeats = floor((m->getLength()/1000.0f) * (bpm/60.0f));
   
   if (stat (filename.c_str(), &buffer) != 0) {
      std::random_device device;
      std::mt19937 gen(device());
      std::bernoulli_distribution coin_flip(0.4);
      
      beat = 5;
      while (beat < totalBeats) {
         Event e;
         beat += (rand()%4) + 1;
         bool outcome = coin_flip(gen);
         
         if (outcome){
            e.object = WALL;
         }
         else {
            e.object = METEOR;
         }
         
         lane = rand()%TOTAL_OBSTACLES;
         if (e.object == WALL) {
            e.lane = GREEN;
         }
         else {
            e.lane = lane;
         }
         e.color = lane;

         events.emplace(beat, e);
      }
   }
   else {
      std::ifstream infile(filename);
      
      std::string songName = "";
      int bpm = 0;
      int eq1, eq2, eq3, eq4, eq5, eq6 = 0;
      
      // INSIST that each beatmap has a song name.
      assert(getline(infile, songName));
      
      // Get the BPM
      infile >> bpm;
      // Get the track wave equation values from the beatmap
      infile >> eq1 >> eq2 >> eq3 >> eq4 >> eq5 >> eq6;
      
      while (infile >> beat >> obj >> lane >> color) {
         if (beat <= totalBeats) {
            Event e;
            e.object = obj;
            e.lane = lane;
            e.color = color;
            events.emplace(beat, e);
         }
      }
   }
}

void BeatEventListener::update(int currBeat, State* world) {
   InGameState* igs = dynamic_cast<InGameState *>(world);
   if (igs != nullptr) {
      // Account for all beats that might have been skipped
      if (lastBeat > currBeat) 
         lastBeat = currBeat;
      if (lastBeat + SPAWN_OFFSET < currBeat)
         lastBeat = currBeat - SPAWN_OFFSET;

      float beat_time = 60.0f / float(igs->getSoundtrack()->getBPM());
      float beat_offset = beat_time * igs->getSoundtrack()->getBeatOffsetMS() / 1000;
      for (; lastBeat <= currBeat; lastBeat++) {
         Beat beats_left = (lastBeat + SPAWN_OFFSET) - currBeat;
         float travel_time = beat_time * beats_left - beat_offset;

         if (events.find(lastBeat + SPAWN_OFFSET) != events.end()) {
            Event curr_event = events[lastBeat + SPAWN_OFFSET];

            // Add object to hit in SPAWN_OFFSET beats
            igs->addObstacle(static_cast<Track>(curr_event.lane),
               static_cast<Track>(curr_event.color),
               ObstacleType(curr_event.object),
               travel_time);
         }
         
         if (lastBeat > 0) {
            // Add object to hit in SPAWN_OFFSET beats
            igs->addGate(travel_time);
         }
      }
   }
}