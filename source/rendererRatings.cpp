#include "rendererRatings.h"
#include "main.h"

bool RendererRatings::initialized = false;

static float STAR_WIDTH = 31.0f / 128.0f;
static float STAR_HEIGHT = 28.0f / 128.0f;

void RendererRatings::init() {
   helper = new Renderer2D("./textures/ratings.png", true, 0.0f);
   
   GLenum error = glGetError();
   assert(error == 0);
   
   initialized = true;
}

void RendererRatings::addRating(glm::vec2 topLeft, Rating rating_type, glm::vec2 font_size) {
//   const float font_spacing = font_size.x / 2.0f;
//   
//   //topLeft.x -= float(message.length()) * font_spacing / 2;
//   topLeft.y -= font_size.y / 2;

   positions.push_back(topLeft + glm::vec2(0, font_size.y));
   positions.push_back(topLeft + glm::vec2(font_size.x, 0));
   uvs.push_back(getRatingsUV(rating_type));
   uvs.push_back(getRatingsUV(rating_type) + glm::vec2(STAR_WIDTH, STAR_HEIGHT));
   opacities.push_back(1.0f);
   opacities.push_back(1.0f);
}

glm::vec2 RendererRatings::getRatingsUV(Rating rating_type) {
   float x_value = 0.0;
   
   switch (rating_type) {
      case Half:
         x_value = 38.0f / 128.0f;
         break;
      case Empty:
         x_value = 76.0f / 128.0f;
         break;
      default:
         break;
   }
   
   return glm::vec2(x_value, 0);
}

RendererRatings::RendererRatings() {
   if (!initialized)
      init();
   
   clearAllRatings();
};

RendererRatings *RendererRatings::instance() {
   static RendererRatings *inst = new RendererRatings();
   return inst;
}

void RendererRatings::clearAllRatings() {
   positions.clear();
   uvs.clear();
   opacities.clear();
}

void RendererRatings::updateBuffers() {
   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
   helper->bufferData(Opacities, opacities);
}