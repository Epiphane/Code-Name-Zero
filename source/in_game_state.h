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
#include "audio_visualizer.h"
#include "state.h"
#include "HUD.h"
#include "track_manager.h"
#include "track_enum.h"

#define NUM_TRACKS 3

class ShadowMap;
//circular dependencies are silly
class TrackManager;
class BeatEventListener;


class InGameState : public State {
private:
   GameObject *player;
   ShadowMap *shadowMap;
   Music *soundtrack;
   TrackManager *track_manager;
   BeatEventListener *event_listener;
   AudioVisualizer *visualizer;

   HUD *hud;
   std::vector<std::list<GameObject *>> obstacleLists;
   
   void cleanupObstacles();
public:
   InGameState();
   ~InGameState();
   
   void start();
   
   GameObject *getPlayer() { return player; }
   Music *getSoundtrack() { return soundtrack; }
   TrackManager *getTrackManager() {return track_manager;};
   
   void collide(GameObject *player, GameObject *other);
   void update(float dt);
   void render(float dt);
   
   void send(std::string message, void *data);
   
   GameObject *addObstacle(glm::vec3 position, int track_num, Track track, Track color, int obj, int spawntime, int hittime);
};

glm::vec3 getPlayerPosition();
float getPlayerLatPosition();

#endif /* defined(__Project__in_game_state__) */
