//
//  track_enum.h
//  RGBZero
//
//  Created by David Ellison on 5/12/15.
//
//

#ifndef RGBZero_track_enum_h
#define RGBZero_track_enum_h

enum Track {
   BLUE = 0,
   GREEN = 1,
   RED = 2
};

Track getTrackFromLatPos(float latPos);

const int TOTAL_OBSTACLES = 3;
enum ObstacleType {
   METEOR = 0,
   WALL = 1,
   SPIKE = 2
};

#endif
