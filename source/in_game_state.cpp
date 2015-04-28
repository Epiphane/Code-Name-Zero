//
//  world.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include <fstream>
#include <sstream>
#include <string>

#include "input_manager.h"
#include "audio_manager.h"
#include "in_game_state.h"
#include "camera.h"
#include "renderer.h"
#include "main.h"

// Farthest Z value of track
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
   player->getGraphics()->getRenderer(0)->mat = MATERIAL_BLUE;
   movement->setSpeed(camera_getLookAt()*100.0f);
   addObject(player);
   
   //GameObject *obj = new GameObject(new ModelRenderer("models/bunny.obj"));
   //addObject(obj);
   
   input_set_callback(GLFW_KEY_P, switchModels);
   
   camera_follow(player, glm::vec3(0, 1, 4));
   
   //GameObject *ground = new GameObject(new GroundRenderer(GROUND_WIDTH/2));
   //addObject(ground);
   
   // NOTE:
   // 10 Segments of track seems to be the magic number, 27.5 units long
   for (int i=0; i<15; i++) {
      GameObject *track = new GameObject(new ModelRenderer("models/RGB_track.obj", "models/"));
      track->transform(glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f));
      track->setPosition(glm::vec3(0.0f,0.0f,i*-27.5f));
      track->getGraphics()->getRenderer(0)->mat = MATERIAL_METAL;
      addObject(track);
      track_segments.push_back(track);
      track_length-=27.5f;
   }
   
   soundtrack = audio_load_music("./audio/RGB_Happy_Electro.mp3", 120);
   soundtrack->play();
}

void InGameState::send(std::string message, void *data) {
   if (message == "beat") {
//      std::cout << "beat " << *(Beat *)data << std::endl;
   }
}

void InGameState::update(float dt) {
   // Update all objects and the camera
   State::update(dt);

   float player_z = player->getPosition().z;
   
   if (player_z <= track_segments[1]->getPosition().z) {
      GameObject *track = new GameObject(new ModelRenderer("models/RGB_track.obj", "models/"));
      track->transform(glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f));
      track->setPosition(glm::vec3(0.0f,0.0f,track_length));
      track->getGraphics()->getRenderer(0)->mat = MATERIAL_METAL;
      addObject(track);
      track_segments.push_back(track);
      track_length-=27.5f;
      
      track_segments[0]->die();
      track_segments.erase(track_segments.begin());
   }
   
   
}

float elapsed[25] = {1};
int pos = 0;
void InGameState::render(float dt) {

   State::render(dt);
}
