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
#include <memory>

#include "input_manager.h"
#include "audio_manager.h"
#include "in_game_state.h"
#include "pause_state.h"
#include "camera.h"
#include "renderer.h"
#include "rendererPostProcess.h"
#include "rendererDebug.h"
#include "main.h"
#include "track_manager.h"
#include "shadowMap.h"
#include "beatEventListener.h"
#include "rendererSky.h"

#include "score_state.h"

#define Z_EPSILON 5.0

ShipModel* playerShip;

void toggleDebug() {
   PAUSED = !PAUSED;
   
   audio_setPaused(PAUSED);
   camera_setDebug(PAUSED);
}

InGameState::InGameState(std::string levelname, Beat bpm, int player_ship) : level(levelname), player_speed(100), sun_rotation(-45.0f), score(0) {
   State::State();
   
   playerShipIndex = player_ship;
   playerShip = ShipManager::instance()->getModel(player_ship);
   
   // Move camera
   camera_init(glm::vec3(0, 2, 0), glm::vec3(0, 2, -10));
   
   // Create player ships
   player_movement = new PlayerPhysicsComponent();
   player = std::make_shared<GameObject>(playerShip->getModelRenderer(),
                      player_movement,
                      new PlayerInputComponent,
                      new PlayerCollisionComponent(player_ship));
   
   addObject(player);
   
   camera_follow(player, glm::vec3(0, 1, 7));
   
   // Set up track manager
   track_manager = new TrackManager();

   soundtrack = audio_load_music("./audio/" + level + ".mp3", bpm, false);
   
   visualizer = new AudioVisualizer(soundtrack);
   
   hud = new HUD();
   
   //initialize the lists of obstacles
   obstacleLists = std::vector<std::vector<std::shared_ptr<GameObject>>>();
   for (int i = 0; i < NUM_TRACKS; i++) {
      obstacleLists.push_back(std::vector<std::shared_ptr<GameObject>>());
      obstacleLists[i].reserve(32);
   }
   shadowMap = new ShadowMap;
   shadowMap->init(4096);
   
   RendererPostProcess::shaders_init();
   skyRender = new SkyRenderer;
   
   
   std::pair<std::vector<glm::vec3>::iterator, std::vector<glm::vec3>::iterator> iterator
    = playerShip->getExhaustIterator();
   for (; iterator.first != iterator.second; iterator.first++)
   {
      glm::vec3 pos = *(iterator.first);
      ParticleSystem *ps = new ParticleSystem();
      if (!ps->InitParticleSystem(pos)) {
         exit(1);
      }
      particles.push_back(ps);
   }
}

void InGameState::start() {
   State::start();
   
   event_listener = new BeatEventListener;
   event_listener->init("./beatmaps/" + level + ".beatmap", this);
   soundtrack->play();
   
   PAUSED = false;
}

void InGameState::unpause() {
   soundtrack->play();
   input_set_callback(GLFW_KEY_SPACE, toggleDebug);
   input_setMouseLock(true);
}

void InGameState::pause() {
   soundtrack->pause();
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
      collide(player.get(), obstacleLists[getTrackFromLatPos(latPos)].front().get());
   }

   for (int i = 0; i < NUM_TRACKS; i++) {
      while (!obstacleLists[i].empty() && obstacleLists[i].front()->isDead()) {
         // Decelerate player for missing an object
         if (obstacleLists[i].begin()->get()->getCollision() != nullptr)
            player_movement->decelerate(playerShip, 2, 0.75f);
         
         obstacleLists[i].erase(obstacleLists[i].begin());
      }
   }
   
   if (soundtrack->getProgress() >= 0.999) {
      setState(new ScoreState(this, level));
   }
   
   if (PAUSED) {
      setState(new PauseState(this));
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
   RendererPostProcess::capture();
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   static int blurRate = 0;
   static float brightness = 0.1f;
   static int previousBeat = (int)soundtrack->getBeat();

   if (!PAUSED) {
	   if (soundtrack->getBeat() != previousBeat) {
		   previousBeat = soundtrack->getBeat();
//		   blurRate += 20;
		   brightness = 0.15f;
	   }
	   blurRate = (player_speed / 40.0f + player_movement->getAccel() * 15.0f + 4 * blurRate) / 5;
	   brightness = std::fmax(brightness - 0.5f * dt, 0.009f);
   }
   else {
	   blurRate = 0;
   }
   
   // Render DA SKY!
   float samples[64];
   soundtrack->getSamples(samples, 64);

   glm::vec4 sunLowAngle(-0.3f, 0.0f, -1.2f, 1.0f);
   float percent_done = (powf(soundtrack->getProgress() - 0.5f, 3.0f) + 0.125f) / 0.25f;
   float angle = 270.0f * percent_done - 45.0f;
   sun_rotation += (angle - sun_rotation) / 16;
   glm::vec3 sunAngle = glm::vec3(glm::rotate(sun_rotation, 0.0f, 0.0f, -1.0f) * sunLowAngle);
   skyRender->render(sunAngle, brightness / 10, samples);

   // Render scene
   track_manager->render();
   State::render(dt);
//   visualizer->render();
   COMPUTE_BENCHMARK(25, "Render elements time: ", true);

   glDisable(GL_DEPTH_TEST);
   for (int i = 0; i < NUM_TRACKS; i++) {
      std::vector<std::shared_ptr<GameObject>>::iterator it = obstacleLists[i].begin();
      while (it < obstacleLists[i].end()) {
         (*it)->renderOutline();
         it ++;
      }
   }
   glEnable(GL_DEPTH_TEST);
   COMPUTE_BENCHMARK(25, "Render outlines time: ", true);
   
   if (!PAUSED) {
      glm::vec3 carPos = player->getPosition();
      glm::mat4 transform = glm::translate(carPos.x, carPos.y, carPos.z);
      std::vector<ParticleSystem *>::iterator iterator = particles.begin();
      while(iterator < particles.end()) {
         ParticleSystem *ps = *iterator;
         ps->UpdateParticles(dt * 1000, player_movement->getSpeed());
         ps->RenderParticles(renderer_getProjection() * camera_getMatrix() * transform, glm::mat4(1.0f), glm::vec3(1.0));
         iterator++;
      }
   }
   COMPUTE_BENCHMARK(25, "Render particles time: ", true);
      
   // Turn off frame buffer, and render frame buffer to screen
   RendererPostProcess::endCapture();
   
   RendererPostProcess::render(blurRate);


   COMPUTE_BENCHMARK(25, "Blur time: ", true);
   // Render non-blurred elements

   hud->render(dt);
//   printf("ComboStreak: %d\n", player_movement->getComboStreak());
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
void InGameState::addObstacle(Track track, Track color, ObstacleType objType, float travel_time) {
   track = color;
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
      case WALL:
         obstacle = "wall";
         position.x = 0;
         break;
      default:
         obstacle = "shieldBall";
         break;
   }
   
   std::string baseDir = "models/obstacles/";
   ObstacleCollisionComponent *occ = new ObstacleCollisionComponent(track, color, objType);
   ObstaclePhysicsComponent *opc = new ObstaclePhysicsComponent;
   opc->init(travel_time);
   std::shared_ptr<GameObject> ob(new GameObject(ModelRenderer::load(baseDir + obstacle + ".obj", baseDir), opc, nullptr, occ, track_manager));
   ob->setPosition(position);
   ob->getGraphics()->tint(color);
   if (objType != WALL)
      ob->setScale(glm::vec3(2));

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
}

void InGameState::addGate(float travel_time) {
   glm::vec3 position(0);
   position.z = -player_speed * travel_time;

   ObstaclePhysicsComponent *opc = new ObstaclePhysicsComponent;
   opc->init(travel_time);
   std::shared_ptr<GameObject> ob(new GameObject(ModelRenderer::load("models/Track/RBG_Gate_1.obj", "models/Track/"), opc, nullptr, nullptr, track_manager));
   
   addObject(ob);
   ob->setPosition(position);
}