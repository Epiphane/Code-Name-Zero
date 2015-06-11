#include "rendererText.h"
#include "main.h"

bool RendererText::initialized = false;

std::vector<glm::vec2> saved_positions, saved_uvs;
std::vector<float> saved_opacities;

void RendererText::init() {
   helper = new Renderer2D("./textures/speed_font.png", true, 0.0f);
   
   GLenum error = glGetError();
   assert(error == 0);
   
   initialized = true;
}

RendererText::RendererText() {
   if (!initialized)
      init();
   
   clearAllText();
};

RendererText *RendererText::instance() {
   static RendererText *inst = new RendererText();
   return inst;
}

void RendererText::addText(glm::vec2 topLeft, std::string message, glm::vec2 font_size, float start_opacity) {
   const float font_spacing = font_size.x / 2.0f;
   
   topLeft.x -= float(message.length()) * font_spacing / 2;
   topLeft.y -= font_size.y / 2;
   for (int i = 0; i < message.length(); i++) {
      // Space out capital letters a bit more
      if (message[i] >= 'A' && message[i] <= 'Z' && i > 0)
         topLeft.x += 0.01f;
      
      positions.push_back(topLeft + glm::vec2(0, font_size.y));
      positions.push_back(topLeft + glm::vec2(font_size.x, 0));
      uvs.push_back(characterUV(message[i]));
      uvs.push_back(characterUV(message[i]) + glm::vec2(1.0f / 16.0f));
      opacities.push_back(start_opacity);
      opacities.push_back(start_opacity);
      
      topLeft.x += font_spacing;
   }
}

void RendererText::clearAllText() {
   positions.clear();
   uvs.clear();
   opacities.clear();
}

void RendererText::clearLastChar() {
   if (positions.size() > 0 && uvs.size() > 0 && opacities.size() > 0) {
      positions.pop_back();
      positions.pop_back();
      uvs.pop_back();
      uvs.pop_back();
      opacities.pop_back();
      opacities.pop_back();
   }
}

void RendererText::saveBuffers() {
   saved_positions = std::vector<glm::vec2>(positions);
   saved_uvs = std::vector<glm::vec2>(uvs);
   saved_opacities = std::vector<float>(opacities);
}

void RendererText::reloadBuffers() {
   clearAllText();
   opacities = std::vector<float>(saved_opacities);
   uvs = std::vector<glm::vec2>(saved_uvs);
   positions = std::vector<glm::vec2>(saved_positions);
}

void RendererText::updateBuffers() {
   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
   helper->bufferData(Opacities, opacities);
}