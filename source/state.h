//
//  state.h
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#ifndef __Project__state__
#define __Project__state__

#include <vector>

#include "game_object.h"

struct Plane {
    float a, b, c, d;
};

class State {
private:
   std::vector<GameObject *> objects;
   std::vector<GameObject *> rendererQueue;
    
   void updateRendererQueue();
   bool toCull(const Plane &plane, GameObject *obj);
   
public:
   State() { objects.clear(); rendererQueue.clear(); }
   
   virtual void start();
   virtual void pause();
   virtual void update(float dt);
   virtual void render(float dt);
   
   virtual void send(std::string message, void *data);
   
   void addObject(GameObject *obj);
   void removeObject(GameObject *obj);
   void collide(GameObject *obj);
};

#endif /* defined(__Project__state__) */
