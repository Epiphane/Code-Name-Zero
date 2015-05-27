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

const int SPAWN_OFFSET = 16;

typedef struct {
   int object;
   int lane;
   int color;
}Event;

class BeatEventListener {
private:
   std::unordered_map<int, Event> events;
   
public:
   BeatEventListener();
   virtual ~BeatEventListener();
   
   void init(std::string filename, State *world);
   void update(int currBeat, State* world);
};

#endif /* defined(__RGBZero__beatEventListener__) */
