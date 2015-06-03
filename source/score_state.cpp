//
//  score_state.cpp
//  Project
//
//  Created by Jonathan Pae on 6/02/15.
//
//

#include "score_state.h"
#include "main.h"

#include <string>
#include <fstream>


ScoreState::ScoreState(State *game) {
   std::string name;
   std::string value;
   
   
   game_state = game;
   
   helper = new Renderer2D("./textures/speed_font.png", true, 0);
   
   positions.clear();
   uvs.clear();
   
   //Display top scores from scores.txt
   std::ifstream infile("./scores/scores.txt");
   while (infile >> name >> value) {
      ScoreEntry se;
      se.name = name;
      while (se.name.length() < MAX_NAME_LENGTH) se.name +=' ';
      se.value = value;
      if (se.value.length() < MAX_SCORE_LENGTH) se.value.insert(0, MAX_SCORE_LENGTH - se.value.length(), ' ');
      scores.push_back(se);
      addText(glm::vec2(0.0, 0.5f), se.name + " : " + se.value);
   }
   
   
   
   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
   helper->bufferData(Opacities, opacities);
}

void ScoreState::addText(glm::vec2 topLeft, std::string message) {
   const glm::vec2 font_size(0.16f);
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
      opacities.push_back(0.5f);
      opacities.push_back(0.5f);
      
      topLeft.x += font_spacing;
   }
}

void ScoreState::printScores() {
   std::vector<ScoreEntry>::iterator iterator = scores.begin();
   int counter = 0;
   while(iterator < scores.end()) {
      ScoreEntry se = *iterator;
      std::cout << "Index " << counter++ << " | " << se.name << ":" << se.value << std::endl;
      iterator++;
   }
}

void ScoreState::update(float dt) {
   
}

void ScoreState::render(float dt) {
   game_state->render(dt);
   
   helper->render(glm::mat4(1));
}

void send(std::string message, void *data) {
   
}