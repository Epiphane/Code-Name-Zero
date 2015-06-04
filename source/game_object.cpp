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
#include "rendererDebug.h"

/* Constructors (ew) */
GameObject::GameObject(GraphicsComponent *g) : GameObject(g, NULL, NULL, NULL, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p) : GameObject(g, p, NULL, NULL, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, CollisionComponent *c) : GameObject(g, p, NULL, c, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, InputComponent *i) : GameObject(g, p, i, NULL, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, InputComponent *i, CollisionComponent *c) : GameObject(g, p, i, c, NULL) {};
GameObject::GameObject(GraphicsComponent *g, PhysicsComponent *p, InputComponent *i, CollisionComponent *c, TrackManager *t) 
   : remove(false), position(glm::vec3(0, 0, 0)), scale(glm::vec3(1, 1, 1)), rotation(glm::vec3(0, 0, 0)), Model(glm::mat4(1.0f)), type(OBJECT_OBSTACLE), collidesWith(0), graphics(g), physics(p), input(i), collision(c), transformer(t) {
   children.clear();

   setBounds(g->getBounds());
}

GameObject::~GameObject() {
   delete graphics;
   delete physics;
   delete input;
   delete collision;
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
   glm::mat4 model;
   if (transformer == nullptr)
      model = glm::translate(position);
   else
      model = transformer->translate(position);
   
   // Ryan's curvy track model transforms
   model *= glm::rotate(rotation.z, 0.0f, 0.0f, 1.0f); // Roll
   model *= glm::rotate(-rotation.x, 1.0f, 0.0f, 0.0f); // Pitch
   model *= glm::rotate(rotation.y, 0.0f, 1.0f, 0.0f); // Yaw

   model *= glm::scale(getScale()) * this->Model;
   
   return model;
}

void GameObject::update(State *world, float dt) {
   if (input)
      input->update(this);

   if (physics)
      physics->update(this, world, dt);
}

void GameObject::render() {
   glm::mat4 Model = getModel();
   if (Model == glm::mat4(0)) {
      // Don't render
      return;
   }

   if (graphics)
      graphics->render(Model);
}

void GameObject::renderOutline() {
   glm::mat4 Model = getModel();
   if (Model == glm::mat4(0)) {
      // Don't render
      return;
   }
   
   if (graphics)
      graphics->renderOutline(Model);
}

void GameObject::_debug_render() {
   if (collision) {
      RendererDebug::instance()->renderBounds(position, bounds);
      RendererDebug::instance()->renderCircle(position, getRadius());
   }
}
