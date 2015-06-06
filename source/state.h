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
#include <memory>

#include "game_object.h"

struct Plane {
    float a, b, c, d;
};

class State {
private:
   std::vector<std::shared_ptr<GameObject>> objects;
   std::vector<std::shared_ptr<GameObject>> rendererQueue;
    
   void updateRendererQueue();
   bool toCull(const Plane &plane, GameObject *obj);
   
public:
   State() { objects.clear(); rendererQueue.clear(); }
   
   bool initialized = false;
   virtual void start();
   virtual void pause();
   virtual void unpause();
   virtual void update(float dt);
   virtual void render(float dt);
   virtual void regen_resources();
   
   virtual void send(std::string message, void *data);
   
   void addObject(std::shared_ptr<GameObject>obj);
   void removeObject(std::shared_ptr<GameObject>obj);
   //InGameState defines collide
   virtual void collide(std::shared_ptr<GameObject> obj, std::shared_ptr<GameObject> other) {};
};

#endif /* defined(__Project__state__) */
