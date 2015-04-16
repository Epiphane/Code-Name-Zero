//
//  input_manager.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/15/15.
//
//

#include <iostream>
#include <cassert>

#include "input_manager.h"

GLFWwindow *input_window = NULL;

input_key_callback keyCallbacks[GLFW_KEY_LAST] = {0};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int modes) {
   if (action == GLFW_PRESS && keyCallbacks[key])
      keyCallbacks[key]();
}

void input_init(GLFWwindow *_window) {
   input_window = _window;
   
   glfwSetKeyCallback(input_window, key_callback);
   
   memset(keyCallbacks, 0, sizeof(keyCallbacks));
}

void input_set_callback(int key, input_key_callback cb) {
   if (keyCallbacks[key])
      std::cerr << "WARNING: Overwriting key callback for " << key << std::endl;
   
   keyCallbacks[key] = cb;
}

bool input_keyDown(int key) {
   assert(input_window != NULL);
   
   return glfwGetKey(input_window, key) == GLFW_PRESS;
}