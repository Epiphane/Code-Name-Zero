//
//  beatEventListener.cpp
//  RGBZero
//
//  Created by Max Linsenbard on 5/18/15.
//
//
#include <fstream>
#include <sys/stat.h>
#include <math.h>
#include "beatEventListener.h"
#include "main.h"
#include "audio_manager.h"
#include "in_game_state.h"
#include "track_manager.h"
#include "state.h"

// events.emplace | adds an event
// events[x] | lookup
BeatEventListener::BeatEventListener() {
   
}

BeatEventListener::~BeatEventListener() {
   events.clear();
}

// Beat File Format:
// <Beat# float>,<Object int[1-3]>,<Lane int[1-3]>,<Color int[1-3]>

void BeatEventListener::init(std::string filename) {
   struct stat buffer;
   int beat = 0;
   int obj = 0;
   int lane = 0;
   int color = 0;
   
   InGameState* s = dynamic_cast<InGameState *>(getCurrentState());
   Music* m = s->getSoundtrack();
   int bpm = m->getBPM();
   int totalBeats = floor((m->getLength()/1000.0f) * (bpm/60.0f));
   
   if (stat (filename.c_str(), &buffer) == 0) {
      // File does not exist, create randomly generated beatmap
      while (beat < totalBeats) {
         // TODO: Add stuff to State to set difficulty, then limit randFLoat by that value.
         Event e;
         beat += ceil(randFloat(0.0f, 16.0f));
         e.object = floor(randFloat(1.0f, 3.9f));
         
         if (e.object == OBJ_WALL) {
            e.lane = MID_LANE;
         }
         else {
            e.lane = floor(randFloat(1.0f, 3.9f));
         }
         e.color = floor(randFloat(1.0f, 3.9f));
         
         events.emplace(beat, e);
      }
   }
   else {
      std::ifstream infile(filename);
      
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

void BeatEventListener::update(float dt, unsigned int currBeat, float player_velocity) {
   // Look in map to see if beat we're on has a key assigned.
   if (events.find(currBeat) != events.end() && last_beat != currBeat) {
      // A key:value pair exists for a beat we havent processed yet
      Event curr_event = events[currBeat];
      
      //TODO: figure out how to actually spawn a specific object
      // maybe something like state->addObj(e.obj, e.color, e.lane)
      // All objects need to be monitored in such a way that they will hit the player
      // 4 beats later.... :)
      last_beat = currBeat;
   }
}