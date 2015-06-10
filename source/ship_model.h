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
   ShipModel(std::string name) { fileName = name; }
   
   void addExhuast(glm::vec3 pos);
   std::string getFileName() { return fileName; }
   std::pair<std::vector<glm::vec3>::iterator, std::vector<glm::vec3>::iterator> getExhaustIterator();
   
   GraphicsComponent *getModelRenderer();
   
   void setMakerName(std::string name) { makerName = name; }
   void setEngineName(std::string name) { engineName = name; }
   void setWeight(int weight) { this->weight = weight; }
   void setAccFactor(float accFactor) { this->accFactor = accFactor; }
   
   float getAccFactor() { return accFactor; }
   std::string getEngineName() { return engineName; }
   std::string getMakerName() { return makerName; }
   int getWeight() { return weight; }
   
private:
   std::string fileName;
   std::vector<glm::vec3> exhaustPositions;
   
   //fun misc. facts
   std::string makerName;
   std::string engineName;
   int weight;
   
   float accFactor;
};

#endif /* defined(__RGBZero__ship_model__) */
