//
//  track_enum.cpp
//  RGBZero
//
//  Created by David Ellison on 5/28/15.
//
//

#include <stdio.h>
#include "track_enum.h"

Track getTrackFromLatPos(float latPos) {
   if (latPos < -0.4f) {
      return BLUE;
   } else if (latPos >= -0.4f && latPos <= 0.4f) {
      return GREEN;
   } else {
      return RED;
   }
}