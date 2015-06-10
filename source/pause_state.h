//
//  score_state.h
//  Project
//
//  Created by Jonathan Pae on 6/02/15.
//
//

#ifndef __RGBZero__pause_state__
#define __RGBZero__pause_state__

#include <vector>

#include "rendererText.h"
#include "state.h"
#include "input_manager.h"
#include "in_game_state.h"


class PauseState : public State {
private:
   RendererText *helper;
   State *game_state;
   
   void initializeVariables();
   
public:
   PauseState(State *game);
   
   void update(float dt);
   void render(float dt);
};

#endif /* defined(__RGBZero__pause_state__) */
