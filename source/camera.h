//
//  camera.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/11/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__camera__
#define __FinalProject__camera__

#include <stdio.h>
#include <glm/glm.hpp>
#include <memory>

void camera_setPosition(glm::vec3 position);
void camera_lookAt(glm::vec3 dest);
void camera_setPitch(double pitch);
void camera_movePitch(double dp);
void camera_setYaw(double yaw);
void camera_moveYaw(double dy);
void camera_move(float dx, float dy, float dz);

class GameObject;
class State;
void camera_follow(std::shared_ptr<GameObject> follow, glm::vec3 offset);

void camera_setDebug(bool debug);
void camera_init(glm::vec3 position, glm::vec3 lookAt);
void camera_update(float dt, State *world);

glm::mat4 camera_savedMatrix();
glm::mat4 camera_getMatrix();
glm::vec3 camera_getPosition();
glm::vec3 camera_getLookAt();

#endif /* defined(__FinalProject__camera__) */
