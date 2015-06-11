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
#include "ship_manager.h"
#include "ship_select_state.h"

StaticState::StaticState(std::string background, Music *s, float z) : soundtrack(s) {

   background = "./textures/" + background;
   if (aspect_ratio == 1.6f) {
      background += "_16_10.png";
   }
   else {
      background += "_16_9.png";
   }
   renderer = new Renderer2D(background, true, z);
   
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

StaticState::StaticState(std::string background) : StaticState(background, nullptr, 1.5) {};
StaticState::StaticState(std::string background, float z) : StaticState(background, nullptr, z) {};
StaticState::StaticState(std::string background, Music *s) : StaticState(background, s, 1.5) {};

void StaticState::render(float dt) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   renderer->render(glm::mat4(1));
   State::render(dt);
}

void StaticState::unpause() {
   if (soundtrack) {
      soundtrack->play();
   }
}

void StaticState::pause() {
   if (soundtrack) {
      soundtrack->pause();
   }
}

TitleScreen::TitleScreen() : StaticState("title_screen", audio_load_music("./audio/RGBZeroTitle.mp3", 145, true)) {};

void TitleScreen::update(float dt) {
   if (input_keyDown(GLFW_KEY_SPACE)) {
      setState(new ShipSelect());
   }
}

LoadingScreen::LoadingScreen() : StaticState("loading_screen"), fading_time(0), progress(0) {};

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
   case 9:
   case 10:
   case 11:
   case 12:
   case 13:
      ShipManager::instance()->getModel(num_loaded - 9)->getModelRenderer();
      break;
   case 14:
      break;
   }

   num_loaded++;
}

void LoadingScreen::update(float dt) {
   float progress = float(num_loaded) / num_to_load;
   if (progress == 1) {
      fading_time += dt;
      
      std::vector<float> opacities;
      opacities.push_back(1 - fading_time);
      opacities.push_back(1 - fading_time);
      
      renderer->bufferData(Opacities, opacities);
      
      if (fading_time >= 1) {
         setState(new TitleScreen());
      }
   }
   else {
      loadNext();
   }
}

void LoadingScreen::render(float dt) {
   StaticState::render(dt);
}