//
//  rendererRatings.h
//  RGBZero
//
//  Created by Jonathan Pae on 6/8/15.
//
//

#ifndef __RGBZero__rendererRatings__
#define __RGBZero__rendererRatings__

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <glm/glm.hpp>

#include "renderer.h"
#include "renderer2D.h"

typedef enum {
   Empty,
   Half,
   Full
} Rating;

class RendererRatings {
private:
   Renderer2D *helper;
   
   static bool initialized;
   void init();
   
   RendererRatings();
   glm::vec2 getRatingsUV(Rating rating_type);
   
public:
   static RendererRatings *instance();
   
   std::vector<glm::vec2> positions, uvs;
   std::vector<float> opacities;
   
   void addRating(glm::vec2 topLeft, Rating rating_type, glm::vec2 rating_size);
   
   void clearAllRatings();
   void updateBuffers();
   
   void render() { helper->render(glm::mat4(1.0f)); }
};

#endif /* defined(__RGBZero__rendererRatings__) */
