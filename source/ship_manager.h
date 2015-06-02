//  ship_manager.h
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#ifndef __RGBZero__ship_manager__
#define __RGBZero__ship_manager__


#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string.h>
#include <vector>

#include "ship_model.h"

using namespace std;

#define MAX_SHIPS 5

class ShipManager {
public:
   ShipManager();
   
   ShipModel* getModel(int index) { return models[index]; }
   void initModels();
   
private:
   ShipModel* models[MAX_SHIPS];
   
};

#endif /* defined(__RGBZero__ship_manager__) */
