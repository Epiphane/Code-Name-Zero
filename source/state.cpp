//
//  state.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include "main.h"
#include "camera.h"
#include "state.h"

void State::start() {}
void State::pause() {}

void State::update(float dt) {
   std::vector<GameObject *>::iterator iterator = objects.begin();
   while(iterator < objects.end()) {
      (*iterator)->update(this, dt);
      this->collide(*iterator);
      if ((*iterator)->isDead())
         iterator = objects.erase(iterator);
      else
         iterator ++;
   }
}

void State::render(float dt) {
   std::vector<GameObject *>::iterator iterator;
   for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
      (*iterator)->render();
   }
   
   if (DEBUG) {
      return;
      glm::mat4 View = camera_getMatrix();
      
      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf(&View[0][0]);
      
      std::vector<GameObject *>::iterator iterator;
      for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
         (*iterator)->_debug_render();
      }
   }
}

void State::send(std::string message, void *data) {
   // Do nothing...
}

void State::addObject(GameObject *obj) {
   objects.push_back(obj);
}

void State::removeObject(GameObject *obj) {
   obj->die();
}

void State::collide(GameObject *obj) {
   std::vector<GameObject *>::iterator iterator;
   for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
      if (*iterator != obj) {
         obj->collide(*iterator);
      }
   }
}