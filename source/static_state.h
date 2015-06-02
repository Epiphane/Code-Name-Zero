//
//  static_state.h
//  RGBZero
//
//  Created by Thomas on 6/1/15.
//
//

#ifndef __RGBZero__static_state__
#define __RGBZero__static_state__

#include "state.h"
#include "renderer2D.h"

class StaticState : public State {
protected:
   Renderer2D *renderer;
   
public:
   StaticState(std::string background);
   
   void render(float dt);
};

class LoadingScreen : public StaticState {
private:
   float fading_time, progress;
   
   int loading_pipe[2];
   
public:
   LoadingScreen();
   
   void update(float dt);
   void start();
};

#endif /* defined(__RGBZero__static_state__) */
