#include <glm/glm.hpp>

#include "ship_manager.h"

//"Red Razelle/",
//"Sonic Phantom/",
//"Wild Boar/",
//"Magic Seagull/",
//"Little Wyvern/"

ShipManager *ShipManager::instance() {
   static ShipManager *inst = new ShipManager();
   return inst;
}

ShipManager::ShipManager() {
   ShipModel *one = new ShipModel("Red Razelle/");
   one->addExhuast(glm::vec3(0.05, -1.6, -0.5));
   
   models[0] = one;
   
   ShipModel *two = new ShipModel("Sonic Phantom/");
   two->addExhuast(glm::vec3 (0.05, -1.6, -0.5));
   
   models[1] = two;
   
   ShipModel *three = new ShipModel("Wild Boar/");
   three->addExhuast(glm::vec3 (-0.22, -1.66, -0.5));
   three->addExhuast(glm::vec3 (0.25, -1.66, -0.5));
   
   models[2] = three;
   
   ShipModel *four = new ShipModel("Magic Seagull/");
   four->addExhuast(glm::vec3 (-0.22, -1.0, -0.5));
   four->addExhuast(glm::vec3 (0, -1.0, -0.5));
   four->addExhuast(glm::vec3 (0.25, -1.0, -0.5));
   
   models[3] = four;
   
   ShipModel *five = new ShipModel("Little Wyvern/");
   five->addExhuast(glm::vec3 (-0.55, -1.50, -0.5));
   five->addExhuast(glm::vec3 (0.55, -1.50, -0.5));
   
   models[4] = five;
}