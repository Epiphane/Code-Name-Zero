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
   float speed, latSpeed;
   glm::vec3 direction;
   
public:
   virtual void update(GameObject *obj, State *world, float dt);
   
//   float getSpeed() { return speed; }
//   float getLatSpeed() { return latSpeed; }
//   glm::vec3 getDirection() { return direction; }
   
   void setDirection(glm::vec3 _d) { direction = _d; }
   void setSpeed(float _s) { speed = _s; }
   void setLatSpeed(float _s) { latSpeed = _s; }
};

class PlayerMovementComponent : public MovementComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt);
};

#endif /* defined(__Project__physics_component__) */
