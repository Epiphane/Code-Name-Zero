//
//  beatEventListener.h
//  RGBZero
//
//  Created by Max Linsenbard on 5/18/15.
//
//

#ifndef __RGBZero__beatEventListener__
#define __RGBZero__beatEventListener__

#include <stdio.h>
#include <unordered_map>
#include "main.h"
#include "in_game_state.h"
#include "track_manager.h"
#include "state.h"

const int SPAWN_OFFSET = 4;

typedef struct {
   int object;
   int lane;
   int color;
}Event;

class BeatEventListener {
private:
   std::unordered_map<int, Event> events;
   unsigned int last_beat;
   
public:
   BeatEventListener();
   virtual ~BeatEventListener();
   
   void init(std::string filename);
   void update(float dt, int currBeat, State* world, TrackManager* tm);
};

#endif /* defined(__RGBZero__beatEventListener__) */
