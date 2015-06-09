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

#include "renderer2D.h"
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
   Renderer2D *helper;
   State *game_state;
   std::string level;

   std::vector<ScoreEntry> scores;
   
   std::vector<glm::vec2> positions, uvs;
   std::vector<float> opacities;

   void removeLastText();
   void clearAllText();
   void initializeVariables();
   void addText(glm::vec2 topLeft, std::string message, glm::vec2 font_size);
   void saveToFile();
   
public:
   ScoreState(State *game, std::string level);
   
   void update(float dt);
   void render(float dt);
};

#endif /* defined(__RGBZero__score_state__) */
