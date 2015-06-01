//
//  main.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef FinalProject_main_h
#define FinalProject_main_h

#include <vector>
#include <glm/glm.hpp>

#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "rendererDebug.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef MATH_PI
#define MATH_PI 3.1415926535897932384626433832795
#endif

#define RADIANS_TO_DEG float(180.0f / M_PI)
#define DEG_TO_RADIANS float(M_PI / 180.0f)

#define CAMERA_SPEED 0.005
#define CAMERA_MOVE 0.4

#define FRAMES_PER_SEC 60.0f
#define SEC_PER_FRAME (1 / FRAMES_PER_SEC)

#define GROUND_WIDTH 40
#define MAX_TARGET 50

#define DEBUG_LOG_VAL(val) RendererDebug::instance()->log(std::to_string(val), true);
#define DEBUG_LOG(val) RendererDebug::instance()->log(val, true);
#define INIT_BENCHMARK float _clock = glfwGetTime();
#define COMPUTE_BENCHMARK(samp, msg, everyframe) if (true) {\
   static float _samples[samp] = {1};\
   static int _pos = 0;\
   _samples[_pos] = glfwGetTime() - _clock;\
   _pos = (_pos + 1) % samp;\
   float _elapsed = 0;\
   for (int i = 0; i < samp; i ++)\
      _elapsed += _samples[i];\
   _elapsed = _elapsed / samp;\
   RendererDebug::instance()->log(msg + std::to_string(_elapsed), !everyframe);\
   _clock = glfwGetTime(); /* Chain debugging */ \
   }

extern bool DEBUG;
extern bool isShadowMapRender;
const extern int w_width;
const extern int w_height;

class State;

// Important game functions
void setDebugLog(bool enabled);
void setState(State *state);
State *getCurrentState();

extern float randFloat(float l, float h);
extern glm::vec3 randPoint(float r);
extern glm::vec2 characterUV(char c);

void resize_obj(std::vector<tinyobj::shape_t> &shapes);

// Debugging operators
std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);
std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec);
std::ostream &operator<< (std::ostream &out, const glm::mat4 &vec);

#endif
