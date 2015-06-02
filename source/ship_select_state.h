//
//  title_state.h
//  RGBZero
//
//  Created by David Ellison on 5/26/15.
//
//

#ifndef __RGBZero__title_state__
#define __RGBZero__title_state__

#include "state.h"
#include "ship_manager.h"
#include "game_object.h"


class ShipSelect : public State {
private:
   std::vector<std::string> levelNames;
   ShipManager *shipManager;
   
public:
   ShipSelect();
   //void addObject(GameObject *obj);
   //void removeObject(GameObject *obj);
   
   void start();
   void pause();
   void update(float dt);
   void render(float dt);

   
   std::string getLevelName();
};

#endif /* defined(__RGBZero__title_state__) */
