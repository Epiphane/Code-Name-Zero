#include <iostream>

#include "ship_model.h"

void ShipModel::addExhuast(glm::vec3 pos) {
   exhaustPositions.push_back(pos);
}

std::pair<std::vector<glm::vec3>::iterator, std::vector<glm::vec3>::iterator> ShipModel::getExhaustIterator() {
   return std::make_pair(exhaustPositions.begin(), exhaustPositions.end());
}

GraphicsComponent *ShipModel::getModelRenderer() {
   return ModelRenderer::load("models/" + fileName + "/model.obj", "models/" + fileName + "/");
}

std::string ShipModel::getFontString() {
   unsigned char a = icon_ndx * 2 + 16;
   unsigned char b = a + 1;

   return std::string(1, a) + std::string(1, b) + " ";
}