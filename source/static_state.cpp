//
//  static_state.cpp
//  RGBZero
//
//  Created by Thomas on 6/1/15.
//
//

#include <errno.h>

#include "main.h"
#include "tutorial_state.h"
#include "static_state.h"

StaticState::StaticState(std::string background) {
   renderer = new Renderer2D(background, true, 0);
   
   std::vector<glm::vec2> positions, uvs;
   positions.push_back(glm::vec2(-1, 1));
   positions.push_back(glm::vec2(1, -1));
   uvs.push_back(glm::vec2(0));
   uvs.push_back(glm::vec2(1));
   
   std::vector<float> opacities;
   opacities.push_back(1);
   opacities.push_back(1);
   
   renderer->bufferData(Opacities, opacities);
   renderer->bufferData(Vertices, positions);
   renderer->bufferData(UVs, uvs);
}

void StaticState::render(float dt) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   renderer->render(glm::mat4(1));
   GLenum error = glGetError();
   assert(error == 0);
}

LoadingScreen::LoadingScreen() : StaticState("./textures/loading_screen.png"), fading_time(0), progress(0) {
};


void loadObstacleModel(std::string obstacle, std::string extension) {
   std::string baseDir = "models/obstacles/" + obstacle + "_" + extension + "/";
   ModelRenderer::load(baseDir + obstacle + "_" + extension + ".obj", baseDir);
}

void LoadingScreen::loadNext() {
   switch (num_loaded) {
   case 0:
      loadObstacleModel("wall", "blue");
      break;
   case 1:
      loadObstacleModel("wall", "green");
      break;
   case 2:
      loadObstacleModel("wall", "red");
      break;
   case 3:
      loadObstacleModel("meteor", "blue");
      break;
   case 4:
      loadObstacleModel("meteor", "green");
      break;
   case 5:
      loadObstacleModel("meteor", "red");
      break;
   case 6:
      loadObstacleModel("obstacle2", "blue");
      break;
   case 7:
      loadObstacleModel("obstacle2", "green");
      break;
   case 8:
      loadObstacleModel("obstacle2", "red");
      break;
   }

   num_loaded++;
}

void LoadingScreen::update(float dt) {
   float progress = float(num_loaded) / num_to_load;
   std::cout << "Progress: " << progress << std::endl;
   if (progress == 1) {
      fading_time += dt;
      
      std::vector<float> opacities;
      opacities.push_back(1 - fading_time);
      opacities.push_back(1 - fading_time);
      
      renderer->bufferData(Opacities, opacities);
      
      if (fading_time >= 1) {
         setState(new TutorialState(4));
      }
   }
   else {
      loadNext();

   }
}