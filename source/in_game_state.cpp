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

const int InGameState::NUM_SHIPS = 5;
const std::string InGameState::SHIP_MODELS[] = {
   "Red Razelle/",
   "Sonic Phantom/",
   "Wild Boar/",
   "Magic Seagull/",
   "Little Wyvern/"
};

void loadObstacleModel(std::string obstacle, std::string extension) {
   std::string baseDir = "models/obstacles/" + obstacle + "_" + extension + "/";
   ModelRenderer::load(baseDir + obstacle + "_" + extension + ".obj", baseDir);
}

InGameState::InGameState(std::string levelname, Beat bpm, int player_ship) : level(levelname), player_speed(100), score(0), sun_rotation(-45.0f) {
   State::State();
   
   // Move camera
   camera_init(glm::vec3(0, 2, 0), glm::vec3(0, 2, -10));
   
   // Create player ships
   std::string ship = "models/" + SHIP_MODELS[player_ship];
   
   player_movement = new PlayerPhysicsComponent();
   player = new GameObject(ModelRenderer::load(ship + "model.obj", ship),
                           player_movement,
                           new PlayerInputComponent,
                           new PlayerCollisionComponent);
   addObject(player);
   
   camera_follow(player, glm::vec3(0, 1, 4));
   
   // Set up track manager
   track_manager = new TrackManager();

   soundtrack = audio_load_music("./audio/" + level + ".mp3", bpm);
   
   visualizer = new AudioVisualizer(soundtrack);
   
   hud = new HUD();
   
   //initialize the lists of obstacles
   obstacleLists = std::vector<std::vector<GameObject *>>();
   for (int i = 0; i < NUM_TRACKS; i++) {
      obstacleLists.push_back(std::vector<GameObject *>());
      obstacleLists[i].reserve(32);
   }
   shadowMap = new ShadowMap;
   shadowMap->init(4096);
   
   RendererPostProcess::shaders_init();
   skyRender = new SkyRenderer;
   
   skyRender = new SkyRenderer;
   ps = new  ParticleSystem();
   if (!ps->InitParticleSystem(glm::vec3(0.05, -1.6, -0.5))) {
      exit(1);
   }

   // Preload models
   loadObstacleModel("wall", "blue");
   loadObstacleModel("wall", "green");
   loadObstacleModel("wall", "red");
   loadObstacleModel("meteor", "blue");
   loadObstacleModel("meteor", "green");
   loadObstacleModel("meteor", "red");
   loadObstacleModel("obstacle2", "blue");
   loadObstacleModel("obstacle2", "green");
   loadObstacleModel("obstacle2", "red");
}

void InGameState::start() {
   event_listener = new BeatEventListener;
   event_listener->init("./beatmaps/" + level + ".beatmap", this);

   soundtrack->play();
}

InGameState::~InGameState() {
   delete shadowMap;
}

void InGameState::send(std::string message, void *data) {
   if (message == "beat") {
      event_listener->update(*(Beat *)data, this);
   }
}

void InGameState::update(float dt) {
   player_speed = player_movement->getSpeed();
   score += player_speed * SCORE_MULT * dt;

   // Update all objects and the camera
   State::update(dt);
   
   hud->update(dt, this);
   
   visualizer->update(dt);

   track_manager->update(dt, this);
   
   float latPos = player->getPosition().x;
   if (!obstacleLists[getTrackFromLatPos(latPos)].empty()) {
      collide(player, obstacleLists[getTrackFromLatPos(latPos)].front());
   }

   for (int i = 0; i < NUM_TRACKS; i++) {
      while (!obstacleLists[i].empty() && obstacleLists[i].front()->isDead()) {
         obstacleLists[i].erase(obstacleLists[i].begin());
      }
   }
   
   ps->UpdateParticles(dt * 1000, player_movement->getSpeed());
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

   static int blurRate = 0;
   static float brightness = 0.1f;
   static int previousBeat = (int)soundtrack->getBeat();

   if (!DEBUG) {
	   if (soundtrack->getBeat() != previousBeat) {
		   previousBeat = soundtrack->getBeat();
//		   blurRate += 20;
		   brightness = 0.15f;
	   }
	   blurRate = (player_speed / 10.0f + player_movement->getAccel() * 60.0f + 9 * blurRate) / 10;
	   brightness = std::fmax(brightness - 0.5f * dt, 0.009f);
   }
   else {
	   blurRate = 0;
   }
   
   // Render DA SKY!
   glm::vec4 sunLowAngle(-0.3f, 0.0f, -0.7f, 1.0f);
   float percent_done = (powf(soundtrack->getProgress() - 0.5f, 3.0f) + 0.125f) / 0.25f;
   float angle = 270.0f * percent_done - 45.0f;
   sun_rotation += (angle - sun_rotation) / 16;
   glm::vec3 sunAngle = glm::vec3(glm::rotate(sun_rotation, 0.0f, 0.0f, -1.0f) * sunLowAngle);
   skyRender->render(sunAngle, brightness);

   // Render scene
   track_manager->render();
   State::render(dt);
   visualizer->render();
   if (DEBUG)
      RendererDebug::instance()->render(glm::mat4(1));
   COMPUTE_BENCHMARK(25, "Render elements time: ", true)
   
   glDisable(GL_DEPTH_TEST);
   for (int i = 0; i < NUM_TRACKS; i++) {
      std::vector<GameObject *>::iterator it = obstacleLists[i].begin();
      while (it < obstacleLists[i].end()) {
         (*it)->renderOutline();
         it ++;
      }
   }
   glEnable(GL_DEPTH_TEST);
   COMPUTE_BENCHMARK(25, "Render outlines time: ", true)
   
   if (!DEBUG) {
      glm::vec3 carPos = player->getPosition();
      glm::mat4 transform = glm::translate(carPos.x, carPos.y, carPos.z);
      ps->RenderParticles(renderer_getProjection() * camera_getMatrix() * transform, glm::mat4(1.0f), glm::vec3(1.0));
   }
   COMPUTE_BENCHMARK(25, "Render particles time: ", true)
      
   // Turn off frame buffer, and render frame buffer to screen
   RendererPostProcess::endCapture();
   
   RendererPostProcess::render(blurRate);
   COMPUTE_BENCHMARK(25, "Blur time: ", true)
   
   // Render non-blurred elements
   hud->render(dt);
   //visualizer->render();
   
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
GameObject *InGameState::addObstacle(Track track, Track color, ObstacleType objType, float travel_time) {
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

   switch (objType) {
      case METEOR:
         obstacle = "meteor";
         break;
      case WALL:
         obstacle = "wall";
         position.x = 0;
         break;
      case SPIKE:
         obstacle = "obstacle2";
         break;
   }
   
   std::string baseDir = "models/obstacles/" + obstacle + "_" + extension + "/";
   ObstacleCollisionComponent *occ = new ObstacleCollisionComponent(track, color, objType);
   ObstaclePhysicsComponent *opc = new ObstaclePhysicsComponent;
   opc->init(travel_time);
   GameObject *ob = new GameObject(ModelRenderer::load(baseDir + obstacle + "_" + extension + ".obj", baseDir), opc, nullptr, occ, track_manager);
   ob->setPosition(position);
   ob->getGraphics()->tint(color);

   //set its position and let the world know it exists
   addObject(ob);
   
   if (objType == WALL) {
      obstacleLists[BLUE].push_back(ob);
      obstacleLists[GREEN].push_back(ob);
      obstacleLists[RED].push_back(ob);
   }
   else {
      obstacleLists[track].push_back(ob);
   }
   
   return ob;
}

GameObject *InGameState::addGate(float travel_time) {
   glm::vec3 position(0);
   position.z = -player_speed * travel_time;

   ObstaclePhysicsComponent *opc = new ObstaclePhysicsComponent;
   opc->init(travel_time);
   GameObject *ob = new GameObject(ModelRenderer::load("models/Track/RBG_Gate_1.obj", "models/Track/"), opc, nullptr, nullptr, track_manager);
   
   addObject(ob);
   ob->setPosition(position);
   
   return ob;
}