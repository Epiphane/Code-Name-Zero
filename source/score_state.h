//
//  score_state.h
//  Project
//
//  Created by Jonathan Pae on 6/02/15.
//
//

#ifndef __RGBZero__score_state__
#define __RGBZero__score_state__

#include <vector>
#include <string>

#include "rendererText.h"
#include "state.h"
#include "input_manager.h"
#include "in_game_state.h"

#define MAX_SCORE_ENTRY 5
#define MAX_NAME_LENGTH 10
#define MAX_SCORE_LENGTH 6

struct ScoreEntry {
   std::string name; // Cannot be deliminated by space
   std::string value;
   std::string ship;
};

class ScoreState : public State {
private:
   RendererText *helper;
   State *game_state;
   std::string level;

   std::vector<ScoreEntry> scores;
   
   void initializeVariables();
   void saveToFile();
   
public:
   ScoreState(State *game, std::string level);
   
   void update(float dt);
   void render(float dt);
};

#endif /* defined(__RGBZero__score_state__) */
