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
   void init();
};

class MovementComponent : public PhysicsComponent {
private:
   float velocity = 100.0f;
   glm::vec3 track_position;
   glm::vec3 slide;
   float lat_position = 0, lat_destination = 0;
   glm::vec3 direction;
   float long_position = 0;
   glm::vec3 accel;

public:
   //virtual void update(GameObject *obj, State *world, float dt, TrackManager track);
   virtual void update(GameObject *obj, State *world, float dt);
   glm::vec3 getAccel(){ return accel; }
   void setAccel(glm::vec3 a){ accel = a; }
   
   // Ryan's movement stuff - staying on the moving track
   void setSlide(glm::vec3 a){ slide = a; }
   void setDirection(glm::vec3 a){ direction = a; }
   glm::vec3 getDirection(){ return direction; }
   void setTrackPosition(glm::vec3 a){ track_position = a; }
   void setLongPos(float a){ long_position = a; }
   void setLatPos(float pos, bool instant);
   float getLongPos(){ return long_position; }
   float getLatPos(){ return lat_position; }
   float getVelocity(){ return velocity; }
#define MAX_VELOCITY 1000
   void setVelocity(float a){ if (velocity < MAX_VELOCITY) { velocity = a; } else { velocity = MAX_VELOCITY; } };

};

class PlayerPhysicsComponent : public MovementComponent {
private:
   float accel, decel;
   
public:
   virtual void update(GameObject *obj, State *world, float dt);
   
   void accelerate(float length) { accel = length; }
   void decelerate(float length) { decel = length; }
};

class TrackPhysicsComponent : public PhysicsComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt) {};
};

class ObstaclePhysicsComponent : public MovementComponent {
private:
   float msec_left;
   int lane;
public:
   float getDebug() { return msec_left; }
   void init(int spawn_time, int hit_time, int spawn_lane);
   virtual void update(GameObject *obj, State *world, float dt);
};

#endif /* defined(__Project__physics_component__) */
