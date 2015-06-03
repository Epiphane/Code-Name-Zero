//  ship_model.h
//  RGBZero
//
//  Created by Jonathan Pae on 4/28/15.
//
//

#ifndef __RGBZero__ship_model__
#define __RGBZero__ship_model__


#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "graphics_component.h"

using namespace std;

class ShipModel {
public:
   ShipModel(std::string name) { fileName = "models/" + name; }
   
   void addExhuast(glm::vec3 pos);
   std::string getFileName() { return fileName; }
   std::pair<std::vector<glm::vec3>::iterator, std::vector<glm::vec3>::iterator> getExhaustIterator();
   
   ModelRenderer *getModelRenderer();
   
private:
   std::string fileName;
   std::vector<glm::vec3> exhaustPositions;
};

#endif /* defined(__RGBZero__ship_model__) */
