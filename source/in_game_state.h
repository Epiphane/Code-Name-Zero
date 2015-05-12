//
//  in_game_state.h
//  Project
//
//  Created by Thomas Steinke on 4/14/15.
//
//

#ifndef __Project__in_game_state__
#define __Project__in_game_state__

#include <list>

#include "audio_manager.h"
#include "state.h"
#include "HUD.h"
#include "track_manager.h"

#define NUM_TRACKS 3

class ShadowMap;
class TrackManager;


class InGameState : public State {
private:
   GameObject *player;
   ShadowMap *shadowMap;
   int target_number;
   Music *soundtrack;
   TrackManager *track_manager;

   HUD *hud;
   std::vector<std::list<GameObject *>> obstacleLists;
   
   void cleanupObstacles();
public:
   InGameState();
   ~InGameState();
   
   GameObject *getPlayer() { return player; }
   Music *getSoundtrack() { return soundtrack; }
   
   int getTotalTargets() { return target_number; }
   void collide(GameObject *player, GameObject *other);
   void update(float dt);
   void render(float dt);
   
   void send(std::string message, void *data);
   
   GameObject *addObstacle(glm::vec3 position, Track track, Track color);
   
   void beat(Beat *beat);
};

glm::vec3 getPlayerPosition();
float getPlayerLatPosition();

#endif /* defined(__Project__in_game_state__) */
