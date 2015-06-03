//
//  input_manager.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/15/15.
//
//

#include <iostream>
#include <cassert>

#include "main.h"
#include "input_manager.h"

GLFWwindow *input_window = NULL;
double input_dx, input_dy;

input_key_callback keyCallbacks[GLFW_KEY_LAST] = {0};
input_alpha_callback alphaCallback = nullptr;

GLFWwindow *getWindow() {
	return input_window;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int modes) {
   if (action == GLFW_PRESS && keyCallbacks[key])
      keyCallbacks[key]();
   
   if (alphaCallback && action == GLFW_PRESS && key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
      alphaCallback(key - GLFW_KEY_A + 'A');
   }
}

void input_init(GLFWwindow *_window) {
   input_window = _window;
   
   glfwSetKeyCallback(input_window, key_callback);
   input_dx = input_dy = 0;
   
   memset(keyCallbacks, 0, sizeof(keyCallbacks));
}

void input_on_alphaKey(input_alpha_callback cb) {
   alphaCallback = cb;
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

void input_update() {
   assert(input_window != NULL);
   
   // Update camera
   double xpos, ypos;
   glfwGetCursorPos(input_window, &xpos, &ypos);
   
   input_dx = (double) w_width / 2 - xpos;
   input_dy = (double) w_height / 2 - ypos;
   // Edge case: window initialization
   if (std::abs(input_dx) > 200 || std::abs(input_dy) > 200 || (xpos == 0 && ypos == 0)) {
      input_dx = input_dy = 0;
   }
   
   glfwSetCursorPos(input_window, w_width / 2, w_height / 2);
}

void input_getMouse(double *dx, double *dy) {
   assert(input_window != NULL);
   
   *dx = input_dx;
   *dy = input_dy;
}