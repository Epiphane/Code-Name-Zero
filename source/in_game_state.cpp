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

const float time_per_spawn = 1.0f;
float t = 0;

InGameState::InGameState() {
   State::State();
   
   // Move camera
   camera_init();
   camera_setPosition(glm::vec3(0, 2, 0));
   camera_lookAt(glm::vec3(0, 2, -10));
   
   target_number = 0;
   
   MovementComponent *movement = new PlayerMovementComponent();
   InputComponent *i = new PlayerInputComponent();
   player = new PlayerCollisionComponent();
   
   GameObject *p = new GameObject(new ModelRenderer("models/bluefalcon.obj"),
                                  movement, i, player);
   p->setType(OBJECT_PLAYER);
   p->addCollision(OBJECT_TARGET);
   p->setPosition(glm::vec3(0, 0, 0));
   movement->setDirection(glm::vec3(camera_getLookAt()));
   addObject(p);
   
   //GameObject *ground = new GameObject(new GroundRenderer(GROUND_WIDTH/2));
   //addObject(ground);
   
   GameObject *track = new GameObject(new ModelRenderer("models/RGB_tracksection.obj"));
   track->transform(glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f));
   addObject(track);
   
   GameObject *track2 = new GameObject(new ModelRenderer("models/RGB_tracksection.obj"));
   track2->transform(glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f));
   track2->setPosition(track2->getPosition()+glm::vec3(0.0f,0.0f,-27.5f));
   addObject(track2);
   
   soundtrack = audio_load_music("./audio/RGB_Happy_Electro_End.wav", 120);
   soundtrack->play();
}

void InGameState::send(std::string message, void *data) {
   if (message == "beat") {
//      std::cout << "beat " << *(Beat *)data << std::endl;
   }
}

void InGameState::update(float dt) {
   camera_update();
   
   t += dt;
   if (t >= time_per_spawn && target_number < MAX_TARGET) {
      // Create a new object
      /*MovementComponent *movement = new MovementComponent();
      GameObject *newObject = new GameObject(new ModelRenderer("models/disk_g.obj"),
                                             movement, NULL, new TargetCollisionComponent());
      newObject->getGraphics()->getRenderer(0)->mat = MATERIAL_RUBBER;
      newObject->setType(OBJECT_TARGET);
      newObject->addCollision(OBJECT_TARGET);
      newObject->setPosition(randPoint(GROUND_WIDTH / 3) + glm::vec3(0, 1, 0));
      newObject->transform(glm::rotate(90.0f, 0.0f, 1.0f, 0.0f));
      
      movement->setSpeed(randFloat(5.0f, 10.0f));
      movement->setDirection(glm::vec3(randFloat(-1.0, 1.0), 0, randFloat(-1.0, 1.0)));
      
      addObject(newObject);
      target_number++;*/
      
      t -= time_per_spawn;
   }
   
   State::update(dt);
}

float elapsed[25] = {1};
int pos = 0;
void InGameState::render(float dt) {
   State::render(dt);
   
   char score[32];
   sprintf(score, "Score: %d out of %d", player->score, target_number);
   renderText(score, 50, 700);
   
   elapsed[pos++] = dt;
   pos %= 25;
   float tot = 0;
   for (int i = 0; i < 25; i ++)
      tot += elapsed[i];
   
   sprintf(score, "FPS: %f", 25.0f / tot);
   renderText(score, 50, 650);
}
