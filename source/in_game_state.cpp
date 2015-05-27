//
//  world.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <float.h>
#include <glm/ext.hpp>
#include <fstream>
#include <sstream>
#include <string>

#include "input_manager.h"
#include "audio_manager.h"
#include "in_game_state.h"
#include "camera.h"
#include "renderer.h"
#include "rendererPostProcess.h"
#include "rendererDebug.h"
#include "main.h"
#include "track_manager.h"
#include "shadowMap.h"
#include "beatEventListener.h"
#include "rendererSky.h"

#define Z_EPSILON 5.0

GameObject *playerObj;
int currentPlayerShip = 0;
std::vector<GraphicsComponent *> ships;

glm::vec3 getPlayerPosition();
float getPlayerLatPosition();
Track getTrackFromLatPos(float latPos);

void switchModels() {
   currentPlayerShip = (currentPlayerShip + 1) % ships.size();
   playerObj->setGraphics(ships[currentPlayerShip]);
}

InGameState::InGameState() : player_speed(100) {
   State::State();
   
   // Move camera
   camera_init(glm::vec3(0, 2, 0), glm::vec3(0, 2, -10));
   
   // Create player ships
   std::string model;
   model = "models/Red Razelle/";
   ships.push_back(ModelRenderer::load(model + "model.obj", model));
   model = "models/Sonic Phantom/";
   ships.push_back(ModelRenderer::load(model + "model.obj", model));
   model = "models/Wild Boar/";
   ships.push_back(ModelRenderer::load(model + "model.obj", model));
   model = "models/Magic Seagull/";
   ships.push_back(ModelRenderer::load(model + "model.obj", model));
   model = "models/Little Wyvern/";
   ships.push_back(ModelRenderer::load(model + "model.obj", model));
   
   player_movement = new PlayerPhysicsComponent();
   playerObj = player = new GameObject(ships[currentPlayerShip], 
                                       player_movement, 
                                       new PlayerInputComponent, 
                                       new PlayerCollisionComponent);
   addObject(player);
   
   input_set_callback(GLFW_KEY_P, switchModels);
   
   camera_follow(player, glm::vec3(0, 1, 4));
   
   // Set up track manager
   track_manager = new TrackManager();

   soundtrack = audio_load_music("./audio/BeatBotBlues.mp3", 120);
   soundtrack->play();
   
   visualizer = new AudioVisualizer(soundtrack);
   
   hud = new HUD();
   
   //initialize the lists of obstacles
   obstacleLists = std::vector<std::list<GameObject *>>();
   for (int i = 0; i < NUM_TRACKS; i++) {
      obstacleLists.push_back(std::list<GameObject *>());
   }
   shadowMap = new ShadowMap;
   shadowMap->init(2048);
   
   RendererPostProcess::shaders_init();
   
   skyRender = new SkyRenderer;
}

void InGameState::start() {
   event_listener = new BeatEventListener;
   event_listener->init("./beatmaps/RGB_MuteCity.beatmap");
}

InGameState::~InGameState() {
   delete shadowMap;
   

}

void InGameState::send(std::string message, void *data) {
   if (message == "beat") {
//      std::cout << "beat " << *(Beat *)data << std::endl;
   }
}

void InGameState::update(float dt) {
   player_speed = player_movement->getSpeed();

   // Update all objects and the camera
   State::update(dt);
   
   hud->update(dt, this);
   
   visualizer->update(dt);

   track_manager->update(dt, this);
   
   cleanupObstacles();
   
   event_listener->update(dt, soundtrack->getBeat(), this, track_manager);
   
   float latPos = player->getPosition().x;
   if (!obstacleLists[getTrackFromLatPos(latPos)].empty()) {
      collide(player, obstacleLists[getTrackFromLatPos(latPos)].front());
   }
}

void InGameState::render(float dt) {
   INIT_BENCHMARK
   
   // Render Shadows
   shadowMap->enable();
   isShadowMapRender = true;
   glClear(GL_DEPTH_BUFFER_BIT);
   player->render();
   isShadowMapRender = false;
   shadowMap->disable();
   
   COMPUTE_BENCHMARK(25, "Shadowmap time: ", true)
   
   // Bind Shadow map texture as active
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, shadowMap->getTexID());

   // Turn on frame buffer
   RendererPostProcess::capture();
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   // Render DA SKY!
   // TODO Figure out where and how we want the sun to operate.
   skyRender->render(glm::vec3(0.0f, 0.3f, -0.7f));

   // Render scene
   track_manager->render();
   State::render(dt);
   if (DEBUG)
      RendererDebug::instance()->render(glm::mat4(1));
   COMPUTE_BENCHMARK(25, "Render elements time: ", true)
   
   // Turn off frame buffer, and render frame buffer to screen
   RendererPostProcess::endCapture();

   static int blurRate = 0;
   static float playerPreviousSpeed = 0;

   if (!DEBUG) {
      blurRate = (player_speed / 10.0f + player_movement->getAccel() * 60.0f + 9 * blurRate) / 10;
   }
   else {
      blurRate = 0;
   }
   
   RendererPostProcess::render(blurRate);
   COMPUTE_BENCHMARK(25, "Blur time: ", true)
   
   // Render non-blurred elements
   hud->render(dt);
   visualizer->render();
   
   COMPUTE_BENCHMARK(25, "HUD time: ", true)
}

//this only checks if the player and object overlap in the z-dimension
//in the player's current lane
void InGameState::collide(GameObject *player, GameObject *other) {
   if (other->getCollision()) {
      float playerMinZ = player->getPosition().z + player->getGraphics()->getBounds()->min_z;
      float otherMaxZ = other->getPosition().z + other->getGraphics()->getBounds()->max_z;
      if (playerMinZ < otherMaxZ) {
         player->getCollision()->collide(player, other);
         other->getCollision()->collide(other, player);
      }
   }
}

//add an obstacle to the world.
//params: a vec3 for its position, which Track it's on, and which color it is
GameObject *InGameState::addObstacle(Track track, Track color, int obj, float travel_time) {
   std::string extension;
   std::string obstacle;
   //set up a new obstacle object

   glm::vec3 position(0);
   position.z = -player_speed * travel_time;
   switch (track) {
   case BLUE:
      position.x = -4;
      break;
   case RED:
      position.x = 4;
      break;
   default:
      break;
   }
   
   // Based on color, load different mesh
   switch (color) {
      case BLUE:
         extension = "blue";
         break;
      case GREEN:
         extension = "green";
         break;
      case RED:
         extension = "red";
         break;
   }

   switch (obj) {
      case METEOR:
         obstacle = "meteor";
         break;
      case WALL:
         obstacle = "wall";
         break;
      case SPIKE:
         obstacle = "obstacle2";
         position += glm::vec3(0, 2, 0);
         break;
   }
   
   std::string baseDir = "models/obstacles/" + obstacle + "_" + extension + "/";
//   std::cout << obstacle+ "_" + extension << " spawning in lane " << track << " with color " << color << std::endl;
   ObstacleCollisionComponent *occ = new ObstacleCollisionComponent(track, color);
   ObstaclePhysicsComponent *opc = new ObstaclePhysicsComponent;
   opc->init(travel_time);
   GameObject *ob = new GameObject(ModelRenderer::load(baseDir + obstacle+ "_" + extension + ".obj", baseDir), opc, nullptr, occ, track_manager);

   //set its position and let the world know it exists
   addObject(ob);
   ob->setPosition(position);
   
   if (obj == WALL) {
      obstacleLists[0].push_back(ob);
      obstacleLists[1].push_back(ob);
      obstacleLists[2].push_back(ob);
   }
   else {
      obstacleLists[track].push_back(ob);
   }
   
   return ob;
}

GameObject *InGameState::addGate(glm::vec3 position, int track_num, int spawntime, int hittime) {
   ObstaclePhysicsComponent *opc = new ObstaclePhysicsComponent;
   opc->init(spawntime, hittime, 1);
   GameObject *ob = new GameObject(ModelRenderer::load("models/Track/RBG_Gate_1.obj", "models/Track/"), opc);
   
   addObject(ob);
   ob->setPosition(position);
   
   return ob;
}


//clear the lists of any obstacles behind the player
void InGameState::cleanupObstacles() {
   for (int i = 0; i < NUM_TRACKS; i++) {
      if (!obstacleLists[i].empty()) {
         if (obstacleLists[i].front()->getPosition().z > player->getPosition().z + Z_EPSILON) {
            obstacleLists[i].front()->die();
            obstacleLists[i].pop_front();
         }
      }
   }
}

//internal only helper function
Track getTrackFromLatPos(float latPos) {
   if (latPos < -0.4f) {
      return BLUE;
   } else if (latPos >= -0.4f && latPos <= 0.4f) {
      return GREEN;
   } else {
      return RED;
   }
}

float getPlayerLatPosition() {
   return playerObj->getPosition().x;
}

glm::vec3 getPlayerPosition() {
   return playerObj->getPosition();
}
