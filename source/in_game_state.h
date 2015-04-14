//
//  in_game_state.h
//  Project
//
//  Created by Thomas Steinke on 4/14/15.
//
//

#ifndef __Project__in_game_state__
#define __Project__in_game_state__

#include "state.h"

class InGameState : public State {
private:
   PlayerCollisionComponent *player;
   int target_number;
   
public:
   InGameState();
   
   int getTotalTargets() { return target_number; }
   void collide(GameObject *obj);
   void update(float dt);
   void render(float dt);
};

#endif /* defined(__Project__in_game_state__) */
