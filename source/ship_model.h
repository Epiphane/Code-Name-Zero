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
private:
   std::string fileName;
   std::vector<glm::vec3> exhaustPositions;
   
   // Spot in the icon sheet
   int icon_ndx;

   //fun misc. facts
   std::string makerName;
   std::string engineName;
   std::string boost;
   int weight;
   int streakNumber;

   float accFactor;

public:
   ShipModel(std::string name, int icon_ndx) : fileName(name), icon_ndx(icon_ndx) {};
   
   void addExhuast(glm::vec3 pos);
   std::pair<std::vector<glm::vec3>::iterator, std::vector<glm::vec3>::iterator> getExhaustIterator();
   
   GraphicsComponent *getModelRenderer();
   
   void setMakerName(std::string name) { makerName = name; }
   void setEngineName(std::string name) { engineName = name; }
   void setWeight(int weight) { this->weight = weight; }
   void setAccFactor(float accFactor) { this->accFactor = accFactor; }
   void setStreakNumber(int streak_number) { this->streakNumber = streak_number; }
   void setBoost(std::string boost) { this->boost = boost; }
   
   std::string getFileName() { return fileName; }
   float getAccFactor() { return accFactor; }
   std::string getEngineName() { return engineName; }
   std::string getMakerName() { return makerName; }
   int getWeight() { return weight; }
   int getStreakNumber() { return streakNumber; }
   std::string getBoostEffect() { return boost; }

   std::string getFontString();
};

#endif /* defined(__RGBZero__ship_model__) */
