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
#include "audio_manager.h"
#include "level_info.h"
#include "renderer2D.h"

class StaticState : public State {
protected:
   Renderer2D *renderer;

   Music *soundtrack;
   
public:
   StaticState(std::string background);
   StaticState(std::string background, float z);
   StaticState(std::string background, Music *soundtrack);
   StaticState(std::string background, Music *soundtrack, float z);
   Renderer2D *getRenderer() { return renderer; }
   void render(float dt);

   void pause();
   void unpause();
};

class TitleScreen : public StaticState {
private:
   static bool loading_screen_loaded;

public:
   TitleScreen();
   
   void update(float dt);
};

class LoadingScreen : public StaticState {
private:
   float fading_time, progress;
   const int num_to_load = 14;
   int num_loaded = -1;
public:
   LoadingScreen();

   void loadNext();
   
   void update(float dt);
   void render(float dt);
};

#endif /* defined(__RGBZero__static_state__) */
