//
//  physics_component.h
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#ifndef __Project__physics_component__
#define __Project__physics_component__

class GameObject;
class State;

/* Physics components require a reference to the world */
class PhysicsComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt) {};
};

class PlayerPhysicsComponent : public PhysicsComponent {
private:
   glm::vec3 speed;
   
public:
   virtual void update(GameObject *obj, State *world, float dt);
   glm::vec3 getSpeed(){return speed;}
   void setSpeed(glm::vec3 s){speed = s;}
};

class TrackPhysicsComponent : public PhysicsComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt) {};
};

#endif /* defined(__Project__physics_component__) */
