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

class TitleScreen : public StaticState {
public:
   TitleScreen() : StaticState("./textures/title_screen.png") {};

   void update(float dt);
};

class LoadingScreen : public StaticState {
private:
   float fading_time, progress;
   const int num_to_load = 9;
   int num_loaded = 0;
   
public:
   LoadingScreen();

   void loadNext();
   
   void update(float dt);
};

#endif /* defined(__RGBZero__static_state__) */
