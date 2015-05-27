//
//  camera.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/11/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <noise.h>
#include <glm/ext.hpp>

#include "main.h"
#include "camera.h"
#include "in_game_state.h"
#include "game_object.h"

#define SHAKE_SCALE 0.02f
noise::module::Perlin shake;
float t = 0;

GameObject *following = nullptr;
glm::vec3 followOffset = glm::vec3(0, 1, 4);

glm::vec3 position;
glm::mat4 transform(1.0f), destination(1.0f);
double pitch, yaw, savedPitch, savedYaw;

glm::vec3 savedPosition, savedLookAt;
glm::mat4 savedTransform, savedDestination;

void camera_setDebug(bool debug) {
   if (debug) {
      savedPosition    = position;
      savedDestination = destination;
      savedTransform   = transform;
      savedLookAt      = camera_getLookAt();
      
      transform = glm::mat4(1.0f);
      savedPitch = pitch;
      savedYaw = yaw;
   }
   else {
      position    = savedPosition;
      destination = savedDestination;
      transform   = savedTransform;
   
      pitch = savedPitch;
      yaw = savedYaw;
   }
}

void camera_init(glm::vec3 _position, glm::vec3 lookAt) {
   position= _position;
   camera_lookAt(lookAt);
}

void camera_follow(GameObject *follow, glm::vec3 offset) {
   following = follow;
   transform = destination = following->getModel();
   followOffset = offset;
}

void camera_setPosition(glm::vec3 _position) {
   position = _position;
}

void camera_update(float dt, State *world) {
   if (DEBUG) {
      if (input_keyDown(GLFW_KEY_A)) {
         camera_move(-CAMERA_MOVE, 0, 0);
      }
      if (input_keyDown(GLFW_KEY_S)) {
         camera_move(0, 0, -CAMERA_MOVE);
      }
      if (input_keyDown(GLFW_KEY_D)) {
         camera_move(CAMERA_MOVE, 0, 0);
      }
      if (input_keyDown(GLFW_KEY_W)) {
         camera_move(0, 0, CAMERA_MOVE);
      }
      if (input_keyDown(GLFW_KEY_Q)) {
         camera_move(0, CAMERA_MOVE, 0);
      }
      if (input_keyDown(GLFW_KEY_E)) {
         camera_move(0, -CAMERA_MOVE, 0);
      }
      
      double dx, dy;
      input_getMouse(&dx, &dy);
      
      camera_movePitch(dy * CAMERA_SPEED);
      camera_moveYaw(dx * CAMERA_SPEED);
   }
   else if (following != nullptr) {
      position = following->getPosition();
      destination = following->getModel();
      
      transform += (destination - transform) * dt * 5;
      
      // Follow the player's direction if it exists
      InGameState *game = dynamic_cast<InGameState *>(world);
      if (game != nullptr) {
         // Shake camera
         t += 0.01f;
         double freq = game->getPlayerSpeed() / 100.0f;
         if (freq > 10.0f) freq = 10.0f;
         shake.SetFrequency(freq);
      }
   }
}

glm::vec3 camera_getPosition() { return position; }
glm::vec3 camera_getLookAt() {
   float y = yaw;
   float p = pitch;
   
   if (!DEBUG && following != nullptr) {
      y += shake.GetValue(0, t, 0) * SHAKE_SCALE;
      p += shake.GetValue(t, 0, 0) * SHAKE_SCALE;
   }
   
   return glm::vec3(cos(y) * cos(p), sin(p), -sin(y) * cos(p));
}

void camera_lookAt(glm::vec3 dest) {
    glm::vec3 dir = glm::normalize(dest - position);
    
    camera_setPitch(asin(dir.y));
    yaw = acos(dir.x / cos(pitch));
    if (dir.z > 0) yaw *= -1;
}

void camera_setPitch(double _pitch) {
    pitch = _pitch;
    if (pitch > M_PI * 5 / 11)
        pitch = M_PI * 5 / 11;
    if (pitch < -M_PI * 5 / 11)
        pitch = -M_PI * 5 / 11;
}

void camera_movePitch(double dp) {
    camera_setPitch(pitch + dp);
}

void camera_setYaw(double _yaw) {
    yaw = _yaw;
}

void camera_moveYaw(double dy) {
    yaw += dy;
}

void camera_move(float dx, float dy, float dz) {
    position += dz * glm::vec3(cos(yaw), 0, -sin(yaw));
    position += dy * glm::vec3(0, 1, 0);
    position += dx * glm::cross(glm::vec3(cos(yaw), 0, -sin(yaw)), glm::vec3(0, 1, 0));
}

glm::mat4 camera_savedMatrix() {
    glm::vec4 pos = glm::vec4(savedPosition, 1);
    glm::vec4 dir = savedTransform * glm::vec4(savedLookAt, 0);
    glm::vec4 up = savedTransform * glm::vec4(0, 1, 0, 0);

    return glm::translate(-followOffset) * glm::lookAt(glm::vec3(pos), glm::vec3(pos + dir), glm::vec3(up));
}

glm::mat4 camera_getMatrix() {
    glm::vec4 pos = glm::vec4(position, 1);
    glm::vec4 dir = transform * glm::vec4(camera_getLookAt(), 0);
    glm::vec4 up = transform * glm::vec4(0, 1, 0, 0);
   
    return glm::translate(-followOffset) * glm::lookAt(glm::vec3(pos), glm::vec3(pos + dir), glm::vec3(up));
}
