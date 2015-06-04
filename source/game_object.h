//
//  gameobject.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//

#ifndef __Project__gameobject__
#define __Project__gameobject__

#include <glm/glm.hpp>
#include <vector>

#include "graphics_component.h"
#include "physics_component.h"
#include "input_component.h"
#include "collision_component.h"
#include "track_manager.h"

#define OBJECT_PLAYER 0
#define OBJECT_TARGET 1
#define OBJECT_OBSTACLE 2

class World;

class GameObject {
private:
   bool remove;
   
   glm::vec3 position, scale, rotation;
   glm::mat4 Model;
   
   unsigned int type, collidesWith;
   
   Bounds bounds;

   // Useless right now, we don't have hierarchical models!
   std::vector<GameObject *> children;

   GraphicsComponent  *graphics;
   PhysicsComponent   *physics;
   InputComponent     *input;
   CollisionComponent *collision;
   TrackManager       *transformer;

public:
   GameObject(GraphicsComponent  *graphics, 
              PhysicsComponent   *physics,
              InputComponent     *input,
              CollisionComponent *collision,
              TrackManager       *trackManager);
   GameObject(GraphicsComponent  *graphics,
              PhysicsComponent   *physics,
              InputComponent     *input,
              CollisionComponent *collision);
   GameObject(GraphicsComponent  *graphics, 
              PhysicsComponent   *physics,
              InputComponent     *input);
   GameObject(GraphicsComponent  *graphics,
              PhysicsComponent   *physics,
              CollisionComponent *collision);
   GameObject(GraphicsComponent  *graphics, 
              PhysicsComponent   *physics);
   GameObject(GraphicsComponent  *graphics);
   ~GameObject();
   
   bool isDead() { return remove; }
   void die()    { remove = true; }
   
   float getRadius();
   glm::mat4 getModel();
   glm::vec3 getPosition() { return position; }
   glm::vec3 getScale()    { return scale;    }
   glm::vec3 getRotation() { return rotation; }
   unsigned int getType()  { return type; }
   
   GraphicsComponent  *getGraphics()  { return graphics; }
   PhysicsComponent   *getPhysics()   { return physics; }
   InputComponent     *getInput()     { return input; }
   CollisionComponent *getCollision() { return collision; }
   
   void setType      (unsigned int _t) { type = _t; }
   void addCollision (unsigned int _t) { collidesWith |= _t; }
   void remCollision (unsigned int _t) { collidesWith &= ~_t; }
   
   void transform   (glm::mat4 _t) { Model = _t * Model; }
   void setPosition (glm::vec3 _p) { position = _p; }
   void setScale    (glm::vec3 _s) { scale    = _s; }
   void setRotation (glm::vec3 _r) { rotation = _r; }
   void setGraphics (GraphicsComponent *g)  { graphics  = g; }
   void setPhysics  (PhysicsComponent *p)   { physics   = p; }
   void setInput    (InputComponent *i)     { input     = i; }
   void setCollision(CollisionComponent *c) { collision = c; }
   void setBounds   (Bounds *_b) {
      memcpy(&bounds, _b, sizeof(Bounds));
   }
   
   //void collide(GameObject *other);
   void update(State *world, float dt);
   void render();
   void renderOutline();
   void _debug_render();
};

#endif
