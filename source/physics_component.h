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

class MovementComponent : public PhysicsComponent {
private:
   glm::vec3 speed, accel;
   
public:
   glm::vec3 getSpeed(){return speed;}
   void setSpeed(glm::vec3 s){speed = s;}
   void setAccel(glm::vec3 a){accel = a;}
   
   virtual void update(GameObject *obj, State *world, float dt);
};

class PlayerPhysicsComponent : public MovementComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt);
};

class TrackPhysicsComponent : public PhysicsComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt) {};
};

#endif /* defined(__Project__physics_component__) */
