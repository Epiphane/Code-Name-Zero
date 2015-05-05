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
#include "rendererDebug.h"
#include "main.h"

// Farthest Z value of track
const float track_piece_length = 30.005f;
float track_length = 0.0f;

GameObject *playerObj;
int currentPlayerShip = 0;
std::vector<GraphicsComponent *> ships;

void switchModels() {
   currentPlayerShip = (currentPlayerShip + 1) % ships.size();
   playerObj->setGraphics(ships[currentPlayerShip]);
}

InGameState::InGameState() {
   State::State();
   
   track_segments.clear();
   
   // Move camera
   camera_init(glm::vec3(0, 2, 0), glm::vec3(0, 2, -10));
   
   target_number = 0;
   
   MovementComponent *movement = new PlayerPhysicsComponent();
   InputComponent *i = new PlayerInputComponent();
   
   // Create player ships
   std::string model;
   model = "models/Red Razelle/";
   ships.push_back(new ModelRenderer(model + "model.obj", model));
   model = "models/Sonic Phantom/";
   ships.push_back(new ModelRenderer(model + "model.obj", model));
   model = "models/Wild Boar/";
   ships.push_back(new ModelRenderer(model + "model.obj", model));
   model = "models/Magic Seagull/";
   ships.push_back(new ModelRenderer(model + "model.obj", model));
   model = "models/Little Wyvern/";
   ships.push_back(new ModelRenderer(model + "model.obj", model));
   
   playerObj = player = new GameObject(ships[currentPlayerShip], movement, i, new PlayerCollisionComponent);
   player->setType(OBJECT_PLAYER);
   player->addCollision(OBJECT_TARGET);
   player->setPosition(glm::vec3(0, 0, 0));
   movement->setSpeed(camera_getLookAt()*100.0f);
   addObject(player);
   
   input_set_callback(GLFW_KEY_P, switchModels);
   
   camera_follow(player, glm::vec3(0, 1, 4));
   
   // NOTE:
   // 10 Segments of track seems to be the magic number, 27.5 units long
   for (int i=0; i<15; i++) {
      GameObject *track = new GameObject(new ModelRenderer("models/Track/RGB_TrackOnly_Curved.obj", "models/Track/"));
      track->setPosition(glm::vec3(0.0f,0.0f,i*-track_piece_length));
      addObject(track);
      track_segments.push_back(track);
      track_length-=track_piece_length;
   }
   
   soundtrack = audio_load_music("./audio/RGB_Happy_Electro.mp3", 120);
   soundtrack->play();
   
   hud = new HUD();
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

   float player_z = player->getPosition().z;
//   std::cout << player_z << std::endl;
   
   if (player_z <= track_segments[1]->getPosition().z) {
      GameObject *track = new GameObject(new ModelRenderer("models/Track/RGB_TrackOnly_Curved.obj", "models/Track/"));
      track->setPosition(glm::vec3(0.0f,0.0f,track_length));
      addObject(track);
      track_segments.push_back(track);
      track_length-=track_piece_length;
      
      track_segments[0]->die();
      track_segments.erase(track_segments.begin());
   }
}

void InGameState::render(float dt) {
   INIT_BENCHMARK

   // Turn on frame buffer
   glBindFramebuffer(GL_FRAMEBUFFER, get_fbo());
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Render scene
   State::render(dt);
   if (DEBUG)
      RendererDebug::instance()->render(glm::mat4(1));
   COMPUTE_BENCHMARK(25, "Render elements time: ", true)
   
   // Turn off frame buffer, and render frame buffer to screen
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   static int blurRate = 0;

   if (!DEBUG) {
      MovementComponent *playMove = (MovementComponent *)player->getPhysics();
	  blurRate = (glm::length(playMove->getSpeed()) / 10.0f + playMove->getAccel().z * 60.0f + 9 * blurRate) / 10;
   }
   else {
      blurRate = 0;
   }
   
   ProgramPostProcrender(blurRate);
   COMPUTE_BENCHMARK(25, "Blur time: ", true)
   
   // Render non-blurred elements
   hud->render(dt);
   
   COMPUTE_BENCHMARK(25, "HUD time: ", true)
}
