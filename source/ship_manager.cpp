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
   one->setEngineName(" CY96-Z1x1");
   one->setMakerName("Cyber Stick, Inc.");
   one->setWeight("1330 kg");
   
   models[0] = one;
   
   ShipModel *two = new ShipModel("Sonic Phantom/");
   two->addExhuast(glm::vec3 (0.05, -1.6, -0.5));
   two->setEngineName("CY97-X4x1");
   two->setMakerName("Abinus Company");
   two->setWeight("1010 kg");
   
   models[1] = two;
   
   ShipModel *three = new ShipModel("Wild Boar/");
   three->addExhuast(glm::vec3 (-0.22, -1.66, -0.5));
   three->addExhuast(glm::vec3 (0.25, -1.66, -0.5));
   three->setEngineName("AM9021-Rx2");
   three->setMakerName("Asteroid Motors");
   three->setWeight("2110 kg");
   
   models[2] = three;
   
   ShipModel *four = new ShipModel("Magic Seagull/");
   four->addExhuast(glm::vec3 (-0.22, -1.0, -0.5));
   four->addExhuast(glm::vec3 (0, -1.0, -0.5));
   four->addExhuast(glm::vec3 (0.25, -1.0, -0.5));
   four->setEngineName("N/A");
   four->setMakerName("Milk Do Shamshu");
   four->setWeight("1330 kg");
   
   models[3] = four;
   
   ShipModel *five = new ShipModel("Little Wyvern/");
   five->addExhuast(glm::vec3 (-0.55, -1.50, -0.5));
   five->addExhuast(glm::vec3 (0.55, -1.50, -0.5));
   five->setEngineName("FGS-C024Mx2");
   five->setMakerName("Space Dynamics");
   five->setWeight("1390 kg");
   
   models[4] = five;
}