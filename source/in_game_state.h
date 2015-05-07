//
//  in_game_state.h
//  Project
//
//  Created by Thomas Steinke on 4/14/15.
//
//

#ifndef __Project__in_game_state__
#define __Project__in_game_state__

#include "audio_manager.h"
#include "state.h"
#include "HUD.h"
#include "track_manager.h"

class InGameState : public State {
private:
   GameObject *player;
   int target_number;
   Music *soundtrack;
   TrackManager *track_manager;

   HUD *hud;
   
public:
   InGameState();
   
   GameObject *getPlayer() { return player; }
   Music *getSoundtrack() { return soundtrack; }
   
   int getTotalTargets() { return target_number; }
   void collide(GameObject *obj);
   void update(float dt);
   void render(float dt);
   
   void send(std::string message, void *data);
   
   void beat(Beat *beat);
};

#endif /* defined(__Project__in_game_state__) */
