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

ShipModel *ShipManager::getModel(std::string name) {
   if (name == "Red Razelle" || name == "Red Gazelle")
      return models[0];
   else if (name == "Sonic Phantom")
      return models[1];
   else if (name == "Wild Boar")
      return models[2];
   else if (name == "Magic Seagull")
      return models[3];
   else if (name == "Little Wyvern")
      return models[4];

   // Model not found
   return models[0];
}

ShipManager::ShipManager() {
   ShipModel *one = new ShipModel("Red Razelle", 0);
   one->addExhuast(glm::vec3(0.05, -1.6, -0.5));
   one->setEngineName("CY96-Z1x1");
   one->setMakerName("Cyber Stick, Inc.");
   one->setWeight(1330);
   one->setAccFactor(1.05f);
   one->setStreakNumber(7);
   one->setBoost(5.25f);
   
   models[0] = one;
   
   ShipModel *two = new ShipModel("Sonic Phantom", 2);
   two->addExhuast(glm::vec3 (0.05, -1.6, -0.5));
   two->setEngineName("CY97-X4x1");
   two->setMakerName("Abinus Company");
   two->setWeight(1010);
   two->setAccFactor(1.20f);
   two->setStreakNumber(10);
   two->setBoost(7.5f);
   
   models[1] = two;
   
   ShipModel *three = new ShipModel("Wild Boar", 4);
   three->addExhuast(glm::vec3 (-0.22, -1.66, -0.5));
   three->addExhuast(glm::vec3 (0.25, -1.66, -0.5));
   three->setEngineName("AM9021-Rx2");
   three->setMakerName("Asteroid Motors");
   three->setWeight(2110);
   three->setAccFactor(0.90f);
   three->setStreakNumber(4);
   three->setBoost(2.25f);
   
   models[2] = three;
   
   ShipModel *four = new ShipModel("Magic Seagull", 3);
   four->addExhuast(glm::vec3 (-0.22, -1.0, -0.5));
   four->addExhuast(glm::vec3 (0, -1.0, -0.5));
   four->addExhuast(glm::vec3 (0.25, -1.0, -0.5));
   four->setEngineName("");
   four->setMakerName("Milk Do Shamshu");
   four->setWeight(1330);
   four->setAccFactor(1.10f);
   four->setStreakNumber(8);
   four->setBoost(6.0f);
   
   models[3] = four;
   
   ShipModel *five = new ShipModel("Little Wyvern", 1);
   five->addExhuast(glm::vec3 (-0.55, -1.50, -0.5));
   five->addExhuast(glm::vec3 (0.55, -1.50, -0.5));
   five->setEngineName("FGS-C024Mx2");
   five->setMakerName("Space Dynamics");
   five->setWeight(1400);
   five->setAccFactor(1.00f);
   five->setStreakNumber(6);
   five->setBoost(4.5f);
   
   models[4] = five;
}