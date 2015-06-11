//
//  song_select_state.h
//  RGBZero
//
//  Created by David Ellison on 6/7/15.
//
//

#ifndef __RGBZero__song_select_state__
#define __RGBZero__song_select_state__

#include <stdio.h>

#include "rendererText.h"
#include "rendererRatings.h"
#include "static_state.h"
#include "level_info.h"

class SongSelect : public StaticState {
public:
   SongSelect(int shipndx);
   void render(float dt);
   void update(float dt);
   void start();
   
private:
   std::string selectedSong;
   std::vector<LevelInfo *> allSongs;
   RendererText *helper;
   void initializeList();
   RendererRatings *ratingsRenderer;
   void updateShipModel(float dt);
   void updateRatings();
   
   void toNextState();
};





#endif /* defined(__RGBZero__song_select_state__) */
