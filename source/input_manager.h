//
//  input_manager.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/15/15.
//
//

#ifndef __RGBZero__input_manager__
#define __RGBZero__input_manager__

#include "GLSL.h"

typedef void (*input_key_callback)(void);
typedef void (*input_alpha_callback)(char);

GLFWwindow *getWindow();

void input_init(GLFWwindow *window);
void input_update();

void input_getMouse(double *dx, double *dy);
void input_on_alphaKey(input_alpha_callback cb);
void input_set_callback(int key, input_key_callback cb);
bool input_keyDown(int key);

#endif /* defined(__RGBZero__input_manager__) */
