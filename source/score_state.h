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

#include "audio_manager.h"
#include "rendererText.h"
#include "renderer2D.h"
#include "state.h"
#include "input_manager.h"
#include "in_game_state.h"
#include "level_info.h"

#define MAX_SCORE_ENTRY 5
#define MAX_NAME_LENGTH 10
#define MAX_SCORE_LENGTH 6

void send_score_request(std::string level);

struct ScoreEntry {
   std::string name; // Cannot be deliminated by space
   std::string value;
   std::string ship;
};

class ScoreState : public State {
private:
   RendererText *helper;
   Renderer2D *icons;
   State *game_state;
   std::string level;
   
   Music *soundtrack;

   std::vector<ScoreEntry> scores;
   
   void initializeVariables();
   void saveToFile();
   
public:
   ScoreState(State *game, std::string level, LevelInfo *levelinfo);
   
   void update(float dt);
   void render(float dt);
   void unpause();
   void pause();
};

#endif /* defined(__RGBZero__score_state__) */
