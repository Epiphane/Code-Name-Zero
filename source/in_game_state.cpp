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
   camera_lookAt(glm::vec3(10, 2, 0));
   
   target_number = 0;
   
   player = new PlayerCollisionComponent();
   GameObject *p = new GameObject(new ModelRenderer("models/car.obj"),
                                  new PlayerMovementComponent(), new PlayerInputComponent(), player);
   p->type = OBJECT_PLAYER;
   p->collidesWith = OBJECT_TARGET;
   p->setY(1);
   p->setDirection(glm::vec3(camera_getLookAt()));
   addObject(p);
   
   GameObject *ground = new GameObject(new GroundRenderer(GROUND_WIDTH/2));
   addObject(ground);
}

void InGameState::update(float dt) {
   camera_update();
   
   if (keysDown[GLFW_KEY_SPACE]) {
      keysDown[GLFW_KEY_SPACE] = 0;
      DEBUG = !DEBUG;
   }
   
   t += dt;
   if (t >= time_per_spawn && target_number < MAX_TARGET) {
      // Create a new object
      GameObject *newObject = new GameObject(new ModelRenderer("models/disk_g.obj"),
                                             new MovementComponent(), new WheelInputComponent(), new TargetCollisionComponent());
      newObject->getGraphics()->getRenderer(0)->mat = MATERIAL_RUBBER;
      newObject->type = OBJECT_TARGET;
      newObject->collidesWith = OBJECT_TARGET;
      newObject->setY(1);
      newObject->setX(randPoint(GROUND_WIDTH/3).x);
      newObject->setZ(randPoint(GROUND_WIDTH/3).z);
      newObject->setSpeed(randFloat(5.0f, 10.0f));
      newObject->rotation.y = 90;
      newObject->setDirection(glm::vec3(randFloat(-1.0, 1.0), 0, randFloat(-1.0, 1.0)));
      
      addObject(newObject);
      target_number++;
      
      t -= time_per_spawn;
   }
   
   State::update(dt);
}

float elapsed[25] = {1};
int pos = 0;
void InGameState::render(float dt) {
   State::render(dt);
   
   char score[16];
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
