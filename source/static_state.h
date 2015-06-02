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
   StaticState(std::string background, float z);
   Renderer2D *getRenderer() { return renderer; }
   void render(float dt);
};

class TitleScreen : public StaticState {
private:
   static bool loading_screen_loaded;

public:
   TitleScreen() : StaticState("./textures/title_screen.png") {};

   void update(float dt);
};

class LoadingScreen : public StaticState {
private:
   float fading_time, progress;
   const int num_to_load = 11;
   int num_loaded = -1;
   int ship;
   State *game = nullptr;
public:
   LoadingScreen(int shipIndex);

   void loadNext();
   
   void update(float dt);
   void render(float dt);
};

#endif /* defined(__RGBZero__static_state__) */
