//
//  level_info.h
//  RGBZero
//
//  Created by David Ellison on 6/11/15.
//
//

#ifndef RGBZero_level_info_h
#define RGBZero_level_info_h


#define TUTORIAL_FILENAME "RGB_Tutorial"

class LevelInfo {
public:
   LevelInfo(std::string fn, std::string st, int beats, bool beatmap);
   //filename without the .mp3
   std::string filename;
   //title from beatmap, or filename if no beatmap
   std::string songTitle;
   //from beatmap
   int bpm;
   //hopefully always true, lol
   bool hasBeatmap;
};

#endif
