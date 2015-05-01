//
//  state.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//
#include <iostream>

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
   
   camera_update(dt);
}

void State::updateRendererQueue() {
   std::vector<GameObject *>::iterator iterator;
   glm::mat4 camMatrix = camera_getMatrix();
   
   rendererQueue.clear();
   Plane top, bottom, right, left, far, near;
   
   if (DEBUG) {
      camMatrix = camera_savedMatrix();
   }
   
   glm::mat4 M = renderer_getProjection() * camMatrix;
   
   top.a = M[0].w - M[0].y;
   top.b = M[1].w - M[1].y;
   top.c = M[2].w - M[2].y;
   top.d = M[3].w - M[3].y;
   
   bottom.a = M[0].w + M[0].y;
   bottom.b = M[1].w + M[1].y;
   bottom.c = M[2].w + M[2].y;
   bottom.d = M[3].w + M[3].y;
   
   right.a = M[0].w - M[0].x;
   right.b = M[1].w - M[1].x;
   right.c = M[2].w - M[2].x;
   right.d = M[3].w - M[3].x;
   
   left.a = M[0].w + M[0].x;
   left.b = M[1].w + M[1].x;
   left.c = M[2].w + M[2].x;
   left.d = M[3].w + M[3].x;
   
   far.a = M[0].w - M[0].z;
   far.b = M[1].w - M[1].z;
   far.c = M[2].w - M[2].z;
   far.d = M[3].w - M[3].z;
   
   near.a = M[0].w + M[0].z;
   near.b = M[1].w + M[1].z;
   near.c = M[2].w + M[2].z;
   near.d = M[3].w + M[3].z;
   
   for(iterator = objects.begin(); iterator < objects.end(); iterator ++) {
      if (!toCull(far, *iterator) && !toCull(near, *iterator) &&
          !toCull(left, *iterator) && !toCull(right, *iterator) &&
          !toCull(bottom, *iterator) && !toCull(top, *iterator)) {
         rendererQueue.push_back(*iterator);
      }
   }
}

bool State::toCull(const Plane &plane, GameObject *obj) {
   glm::vec3 pt = obj->getPosition();
   float dist = plane.a * pt.x + plane.b * pt.y + plane.c * pt.z + plane.d;
   
   return dist + obj->getRadius() * 2 < 0.0;
}

void State::render(float dt) {
   std::vector<GameObject *>::iterator iterator;
   updateRendererQueue();
   
   for(iterator = rendererQueue.begin(); iterator < rendererQueue.end(); iterator ++) {
      (*iterator)->render();
      
      if (DEBUG)
         (*iterator)->_debug_render();
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