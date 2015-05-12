//
//  shadowMap.h
//  RGBZero
//
//  Created by Max Linsenbard on 5/4/15.
//
//

#ifndef __RGBZero__shadowMap__
#define __RGBZero__shadowMap__

#include <stdio.h>
#include "GLIncludes.h"

class ShadowMap {
   GLuint frameBufferID;
   GLuint textureID;
   
   int textureSize;
   
public:
   ShadowMap();
   virtual ~ShadowMap();
   
   void init(int size);
   
   void enable();
   void disable();
   
   GLuint getTexID() {
      return textureID;
   }
};

#endif /* defined(__RGBZero__shadowMap__) */
