#include <glm/glm.hpp>

#include "ship_manager.h"

//"Red Razelle/",
//"Sonic Phantom/",
//"Wild Boar/",
//"Magic Seagull/",
//"Little Wyvern/"

void ShipManager::initModels() {
   ShipModel *one = new ShipModel("Red Razelle/");
   one->addExhuast(glm::vec3(0.05, -1.6, -0.5));
   
   models[0] = one;
   
   ShipModel *two = new ShipModel("Sonic Phantom/");
   two->addExhuast(glm::vec3 (0.05, -1.6, -0.5));
   two->addExhuast(glm::vec3 (0, 0, 0));
   
   models[1] = two;
}