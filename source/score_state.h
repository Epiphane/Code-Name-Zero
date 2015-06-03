//
//  score_state.h
//  Project
//
//  Created by Jonathan Pae on 6/02/15.
//
//

#ifndef __RGBZero__score_state__
#define __RGBZero__score_state__

#include "renderer2D.h"
#include "state.h"

class ScoreState : public State {
private:
   Renderer2D *helper;
   State *game_state;
   
   std::vector<glm::vec2> positions, uvs;
   std::vector<float> opacities;

   
public:
   ScoreState(State *game);
   
   void update(float dt);
   void render(float dt);
};

#endif /* defined(__RGBZero__score_state__) */
