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
   float velocity = 1.0f;
   glm::vec3 track_position;
   glm::vec3 slide;
   float lat_position = 0;
   glm::vec3 direction;
   float long_position = 0;
   glm::vec3 speed, accel;

public:
   //virtual void update(GameObject *obj, State *world, float dt, TrackManager track);
   virtual void update(GameObject *obj, State *world, float dt);
   glm::vec3 getSpeed(){ return speed; }
   glm::vec3 getAccel(){ return accel; }
   void setSpeed(glm::vec3 s){ speed = s; }
   void setAccel(glm::vec3 a){ accel = a; }

   // Ryan's movement stuff - staying on the moving track
   void setSlide(glm::vec3 a){ slide = a; }
   void setDirection(glm::vec3 a){ direction = a; }
   glm::vec3 getDirection(){ return direction; }
   void setTrackPosition(glm::vec3 a){ track_position = a; }
   void setLongPos(float a){ long_position = a; }
   void setLatPos(float a){ lat_position = a; }
   void changeLatPos(float a);
   float getLongPos(){ return long_position; }
   float getLatPos(){ return lat_position; }
   float getVelocity(){ return velocity; }
   glm::vec3 getSlide(){ return slide;};
   void setVelocity(float a){ if (velocity < 2000) { velocity = a; } else { velocity = 1800; } };

};

class PlayerPhysicsComponent : public MovementComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt);
};

class TrackPhysicsComponent : public PhysicsComponent {
public:
   virtual void update(GameObject *obj, State *world, float dt) {};
};

class ObstaclePhysicsComponent : public MovementComponent {
private:
   int spawn_ms;
   int hit_ms;
   float z_position;
   int lane;
public:
   void init(int spawn_time, int hit_time, int spawn_lane);
   virtual void update(GameObject *obj, State *world, float dt);
};

#endif /* defined(__Project__physics_component__) */
