//
//  rendererText.h
//  RGBZero
//
//  Created by Jonathan Pae on 6/8/15.
//
//

#ifndef __RGBZero__rendererText__
#define __RGBZero__rendererText__

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <glm/glm.hpp>

#include "renderer.h"
#include "renderer2D.h"


class RendererText {
private:
   Renderer2D *helper;
   
   std::vector<glm::vec2> positions, uvs;
   std::vector<float> opacities;
   
   static bool initialized;
   void init();
   
   RendererText();
   
public:
   static RendererText *instance();
   
   void addText(glm::vec2 topLeft, std::string message, glm::vec2 font_size = glm::vec2(0.16f));
   void clearAllText();
   void clearLastChar();
   void updateBuffers();
   
   void render() { helper->render(glm::mat4(1.0f)); } 
};

#endif /* defined(__RGBZero__rendererText__) */