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
#include "track_enum.h"
#include "state.h"

BeatEventListener::BeatEventListener() {
   
}

BeatEventListener::~BeatEventListener() {
   events.clear();
}

// Beat File Format:
// <Beat# float>,<Object int[0-3]>,<Lane int[0-3]>,<Color int[0-3]>

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
   
   std::cout << "[BeatEvent-init] Does this even get called " << std::endl;
   
   if (stat (filename.c_str(), &buffer) != 0) {
      // File does not exist, create randomly generated beatmap
      while (beat < totalBeats) {
         std::cout << "[BeatEvent-noFile] Does shit even get called " << std::endl;
         Event e;
         // TODO: Add stuff to State to set difficulty, then limit randFLoat by that value.
         beat += (rand()%16) + 1;
         e.object = rand()%TOTAL_OBSTACLES;
         
         if (e.object == WALL) {
            e.lane = GREEN;
         }
         else {
            e.lane = rand()%TOTAL_OBSTACLES;
         }
         e.color = rand()%TOTAL_OBSTACLES;
         std::cout << "Adding random event to beat " << beat << std::endl;
         // Spawn offset required since we do not want
         events.emplace(beat, e);
      }
   }
   else {
      std::cout << "[BeatEvent-yesFile] Does this even get called " << std::endl;
      std::ifstream infile(filename);
      
      while (infile >> beat >> obj >> lane >> color) {
         if (beat <= totalBeats) {
            Event e;
            e.object = obj;
            e.lane = lane;
            e.color = color;
            std::cout << "Adding event: " << beat << obj << lane << color << std::endl;
            events.emplace(beat, e);
         }
      }
   }
}

void BeatEventListener::update(float dt, int currBeat, State* world, TrackManager* tm) {
   if (events.find(currBeat+SPAWN_OFFSET) != events.end() && last_beat != currBeat) {
      Event curr_event = events[currBeat+SPAWN_OFFSET];
      
      // Spawn object 50 track pieces away (visible tracks/4)
      std::cout << "Beat " << (currBeat+SPAWN_OFFSET) % 4 << "!!" << std::endl;
      // InGameState *world, glm::vec3 trackPos, int track, int color, int obj, int spawntime, int hittime
      InGameState* igs = dynamic_cast<InGameState *>(world);

      int spawnms = (1.0f/(igs->getSoundtrack()->getBPM()/60.0f) * 1000)*currBeat;
      int hitms = (1.0f/(igs->getSoundtrack()->getBPM()/60.0f) * 1000)*(currBeat+SPAWN_OFFSET);
      
      tm->addObstacle(igs,
                      tm->nextPosition(tm->nextTrack_number()),
                      tm->nextTrack_number(),
                      curr_event.lane,
                      curr_event.color,
                      curr_event.object,
                      spawnms,
                      hitms);
      last_beat = currBeat;
   }
}