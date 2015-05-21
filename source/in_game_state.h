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
#include "particle_system.h"

#define NUM_TRACKS 3

class ShadowMap;
class SkyRenderer;
//circular dependencies are silly
class TrackManager;
class BeatEventListener;


class InGameState : public State {
private:
   std::string level;
   
   GameObject *player;
   PlayerPhysicsComponent *player_movement;
   float player_speed;
   
   const static int SCORE_MULT = 1;
   unsigned long score;

   ShadowMap *shadowMap;
   SkyRenderer *skyRender;
   Music *soundtrack;
   TrackManager *track_manager;
   BeatEventListener *event_listener;
   AudioVisualizer *visualizer;
   ParticleSystem *ps;

   HUD *hud;
   std::vector<std::list<GameObject *>> obstacleLists;
   
   void cleanupObstacles();
public:
   const static int NUM_SHIPS;
   const static std::string SHIP_MODELS[];
   
   InGameState(std::string levelname, int playership);
   ~InGameState();
   
   void start();
   
   unsigned long getScore() { return score; }
   
   float getPlayerSpeed() { return player_speed; }
   GameObject *getPlayer() { return player; }
   Music *getSoundtrack() { return soundtrack; }
   TrackManager *getTrackManager() {return track_manager;};
   
   void collide(GameObject *player, GameObject *other);
   void update(float dt);
   void render(float dt);
   
   void send(std::string message, void *data);
   
   GameObject *addObstacle(Track track, Track color, int obj, float travel_time);
   GameObject *addGate(float traveltime);
};

#endif /* defined(__Project__in_game_state__) */
