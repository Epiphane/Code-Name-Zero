//
//  gameobject.h
//  Lab 1 - CPE 476
//  Thomas Steinke & Jonathan Pae
//
#include <iostream>
#include <glm/ext.hpp>

#include "main.h"
#include "state.h"
#include "game_object.h"

/* Constructors (ew) */
GameObject::GameObject(GraphicsComponent *g) : GameObject(g, NULL, NULL, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p) : GameObject(g, p, NULL, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, InputComponent *i) : GameObject(g, p, i, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, InputComponent *i, CollisionComponent *c) : remove(false), position(glm::vec3(0, 0, 0)), scale(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), Model(glm::mat4(1.0f)), type(OBJECT_OBSTACLE), collidesWith(0), graphics(g), physics(p), input(i), collision(c) {
   children.clear();

   setBounds(g->getBounds());
}

float GameObject::getRadius() {
   float max = -1 * bounds.min_x;

   if (max < bounds.max_x)
      max = bounds.max_x;
   if (max < bounds.max_y)
      max = bounds.max_y;
   if (max < bounds.max_z)
      max = bounds.max_z;

   if (max < -1 * bounds.min_y)
      max = -1 * bounds.min_y;
   if (max < -1 * bounds.min_z)
      max = -1 * bounds.min_z;

   assert(max >= 0);

   return max;
}

glm::mat4 GameObject::getModel() {
   glm::mat4 model = glm::translate(position);
   
   MovementComponent *movement = dynamic_cast<MovementComponent *>(physics);
   if (movement != nullptr) {
      glm::vec3 speed = glm::normalize(movement->getSpeed());
      
      float pitch = asin(speed.y);
      model *= glm::rotate(pitch * RADIANS_TO_DEG, 1.0f, 0.0f, 0.0f);
      float yaw = acos(-speed.z / cos(pitch));
      if (speed.x > 0) yaw *= -1;
      model *= glm::rotate(yaw * RADIANS_TO_DEG, 0.0f, 1.0f, 0.0f);
   }
   
   model *= this->Model;
   
   return model;
}

void GameObject::collide(GameObject *other) {
   if ((collidesWith & other->type) == 0)
      return;
   
   if (!collision || !other->collision)
      return;

   float max_dist = getRadius() + other->getRadius();
   float dx = position.x - other->position.x;
   float dy = position.y - other->position.y;
   float dz = position.z - other->position.z;

   // Within each other's sphere
   if (max_dist * max_dist >= dx * dx + dy * dy + dz * dz) {
      // Collided
      collision->collide(this, other);
   }
}

void GameObject::update(State *world, float dt) {
   if (input)
      input->update(this);

   if (physics)
      physics->update(this, world, dt);

   if (collision)
      world->collide(this);
}

void GameObject::render() {
   if (graphics)
      graphics->render(this);
}

void GameObject::_debug_render() {
   if (collision) {
      glPushMatrix();

      glm::mat4 Model = getModel();
      glMultMatrixf(&Model[0][0]);

      _debug_drawBounds(this->bounds);
      _debug_drawSphere(this->getRadius());
      _debug_drawAxis();

      glPopMatrix();
   }
}
