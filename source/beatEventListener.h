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

const int OBJ_WALL = 1;
const int OBJ_METEOR = 2;
const int OBJ_SPIKE = 3;

const int RIGHT_LANE = 1;
const int MID_LANE = 2;
const int LEFT_LANE = 3;

const int BLUE_COLOR = 1;
const int GREEN_COLOR = 2;
const int RED_COLOR = 3;

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
   void update(float dt, unsigned int currBeat, float player_velocity);
};

#endif /* defined(__RGBZero__beatEventListener__) */
