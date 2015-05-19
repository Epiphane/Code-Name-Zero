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

#define Z_EPSILON 5.0

// Farthest Z value of track
// const float track_piece_length = 30.005f;
float track_length = 0.0f;

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

InGameState::InGameState() {
   State::State();
   
   // Move camera
   camera_init(glm::vec3(0, 2, 0), glm::vec3(0, 2, -10));
   
   target_number = 0;
   
   MovementComponent *movement = new PlayerPhysicsComponent();
   InputComponent *i = new PlayerInputComponent();
   
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
   
   playerObj = player = new GameObject(ships[currentPlayerShip], movement, i, new PlayerCollisionComponent);
  // player->setType(OBJECT_PLAYER);
   //player->addCollision(OBJECT_TARGET);
   player->setPosition(glm::vec3(0, 0, 0));
   movement->setSpeed(camera_getLookAt()*100.0f);
   addObject(player);
   
   input_set_callback(GLFW_KEY_P, switchModels);
   
   camera_follow(player, glm::vec3(0, 1, 4));
   
   // Set up track manager
   track_manager = new TrackManager(this, player);

   soundtrack = audio_load_music("./audio/RGB_Persistance.mp3", 200);
   event_listener = new BeatEventListener;
   event_listener->init("./beatmaps/RGB_Persistance.beatmap");
   soundtrack->play();
   
   hud = new HUD();
   
   //initialize the lists of obstacles
   obstacleLists = std::vector<std::list<GameObject *>>();
   for (int i = 0; i < NUM_TRACKS; i++) {
      obstacleLists.push_back(std::list<GameObject *>());
   }
   shadowMap = new ShadowMap;
   shadowMap->init(2048);

   RendererPostProcess::shaders_init();
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
   // Update all objects and the camera
   State::update(dt);
   
   hud->update(dt, this);

   track_manager->update(dt, player->getPosition(), this);
   
   cleanupObstacles();
   
   event_listener->update(dt, soundtrack->getBeat(), dynamic_cast<MovementComponent *>(playerObj->getPhysics())->getVelocity());
   
   float latPos = dynamic_cast<MovementComponent *>(player->getPhysics())->getLatPos();
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
   State::render(dt);
   isShadowMapRender = false;
   shadowMap->disable();
   
   COMPUTE_BENCHMARK(25, "Shadowmap time: ", true)
   
   // Bind Shadow map texture as active
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, shadowMap->getTexID());

   // Turn on frame buffer
//   glBindFramebuffer(GL_FRAMEBUFFER, RendererPostProcess::get_fbo());
   RendererPostProcess::capture();
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Render scene
   State::render(dt);
   if (DEBUG)
      RendererDebug::instance()->render(glm::mat4(1));
   COMPUTE_BENCHMARK(25, "Render elements time: ", true)
   
   // Turn off frame buffer, and render frame buffer to screen
//   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   RendererPostProcess::endCapture();

   static int blurRate = 0;
   static float playerPreviousSpeed = 0;

   if (!DEBUG) {
      MovementComponent *playMove = (MovementComponent *)player->getPhysics();
	  blurRate = (playMove->getVelocity() / 50.0f + (playMove->getVelocity() - playerPreviousSpeed) * 1.0f + 9 * blurRate) / 10;
	  playerPreviousSpeed = playMove->getVelocity();
	  // Old vector based blur rate
//	  blurRate = (glm::length(playMove->getSpeed()) / 10.0f + playMove->getAccel().z * 60.0f + 9 * blurRate) / 10;
   }
   else {
      blurRate = 0;
   }
   
   RendererPostProcess::render(blurRate);
   COMPUTE_BENCHMARK(25, "Blur time: ", true)
   
   // Render non-blurred elements
   //hud->render(dt);
   
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
GameObject *InGameState::addObstacle(glm::vec3 position, Track track, Track color) {
   std::string extension;
   //set up a new obstacle object
   //todo: give it a collision component (and physics component)?
   
   //Switching materials is pretty difficult, so load 3 meshes instead
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
   std::string baseDir = "models/obstacles/obstacle2_" + extension + "/";
   ObstacleCollisionComponent *occ = new ObstacleCollisionComponent(track, color);
   GameObject *obstacle = new GameObject(ModelRenderer::load(baseDir + "obstacle2_" + extension + ".obj", baseDir), occ);

   //set its position and let the world know it exists
   addObject(obstacle);
   obstacle->setPosition(position);
   
   
   obstacleLists[track].push_back(obstacle);
   
   return obstacle;
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
   MovementComponent *movement = dynamic_cast<MovementComponent *>(playerObj->getPhysics());
   return movement->getLatPos();
}

glm::vec3 getPlayerPosition() {
   return playerObj->getPosition();
}