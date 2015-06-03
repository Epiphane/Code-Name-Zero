//
//  score_state.h
//  Project
//
//  Created by Thomas Steinke on 5/30/15.
//
//

#ifndef __RGBZero__score_state__
#define __RGBZero__score_state__

#include "renderer2D.h"
#include "state.h"

class ScoreState : public State {
private:
   Renderer2D *helper;

   
public:
   ScoreState();
   
   void update(float dt);
   void render(float dt);
   
   void send(std::string message, void *data);
};

#endif /* defined(__RGBZero__score_state__) */
