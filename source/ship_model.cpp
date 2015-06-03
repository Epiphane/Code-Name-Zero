#include "ship_model.h"

void ShipModel::addExhuast(glm::vec3 pos) {
   exhaustPositions.push_back(pos);
}

std::pair<std::vector<glm::vec3>::iterator, std::vector<glm::vec3>::iterator> ShipModel::getExhaustIterator() {
   return std::make_pair(exhaustPositions.begin(), exhaustPositions.end());
}

ModelRenderer *ShipModel::getModelRenderer() {
   return ModelRenderer::load(fileName + "model.obj", fileName);
}