//
//  level_info.cpp
//  RGBZero
//
//  Created by David Ellison on 6/11/15.
//
//

#include <stdio.h>
#include <string>
#include "level_info.h"


LevelInfo::LevelInfo(std::string fn, std::string st, int beats, bool beatmap) {
   filename = fn;
   songTitle = st;
   bpm = beats;
   hasBeatmap = beatmap;
}
