//
//  physics_component.h
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#ifndef __Project__physics_component__
#define __Project__physics_component__


#include "ship_model.h"

class GameObject;
class State;

/* Physics components require a reference to the world */
class PhysicsComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt) {};
   void init();
};

class MovementComponent : public PhysicsComponent {
private:
   glm::vec3 speed, accel;

public:
   virtual void update(GameObject *obj, State *world, float dt);

   glm::vec3 getAccel() { return accel; }
   void setAccel(glm::vec3 a) { accel = a; }
};

class PlayerPhysicsComponent : public PhysicsComponent {
private:
   float lat_destination = 0;
   float accel, accel_time;
   float speed = 100;
   int comboStreak = 0;

public:
   static const int min_speed = 300, max_speed = 1400;
   virtual void update(GameObject *obj, State *world, float dt);
   
   void setLatPos(float pos) { lat_destination = pos * 4; }
   float getLatPos() { return lat_destination; }

   void accelerate(ShipModel *playerShip, float time, float accel = 1);
   void decelerate(ShipModel *playerShip, float time, float decel = 1);

   float getSpeed() { return speed; }
   float getAccel() { return accel; }

   int getComboStreak() { return comboStreak; }
   void resetComboStreak() { comboStreak = 0; }
};

class TrackPhysicsComponent : public PhysicsComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt) {};
};

class ObstaclePhysicsComponent : public MovementComponent {
private:
   float sec_left;
public:
   float getTimeLeft() { return sec_left; }

   void init(float travel_time);
   virtual void update(GameObject *obj, State *world, float dt);
};

#endif /* defined(__Project__physics_component__) */
