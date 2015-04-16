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

void input_init(GLFWwindow *window);
void input_set_callback(int key, input_key_callback cb);
bool input_keyDown(int key);

#endif /* defined(__RGBZero__input_manager__) */
