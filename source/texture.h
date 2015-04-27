//
//  texture.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/23/15.
//
//

#ifndef __RGBZero__texture__
#define __RGBZero__texture__

#include "GLSL.h"

#define MAX_TEXTURE_SIZE 512

GLvoid texture_load(std::string filename, int texture);
GLvoid texture_loadToArray(std::string filename, int texture, int layer, int *width, int *height);

#endif /* defined(__RGBZero__texture__) */
