//
//  collision_component.h
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#ifndef __Project__collision_component__
#define __Project__collision_component__

class GameObject;

/* Collision Components react to collision between objects */
class CollisionComponent {
public:
   virtual void collide(GameObject *obj, GameObject *other) {};
};

class TargetCollisionComponent : public CollisionComponent {
public:
   void collide(GameObject *obj, GameObject *other);
};

class PlayerCollisionComponent : public CollisionComponent {
public:
   PlayerCollisionComponent() : score(0) {};
   
   int score;
   void collide(GameObject *obj, GameObject *other);
};

#endif /* defined(__Project__collision_component__) */
