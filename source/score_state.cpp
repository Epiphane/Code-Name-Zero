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


const char *entry_place[] = { "1st: ", "2nd: ", "3rd: ", "4th: ", "5th: " };

ScoreState::ScoreState(State *game) {
   std::string name;
   std::string value;
   int count = 0;
   
   game_state = game;
   
   helper = new Renderer2D("./textures/speed_font.png", true, 0);
   
   positions.clear();
   uvs.clear();
   
   addText(glm::vec2(0.0, 0.75f), "HIGH SCORES", glm::vec2(0.15));
   //Display top scores from scores.txt
   std::ifstream infile("./scores/scores.txt");
   while (infile >> name >> value) {
      ScoreEntry se;
      se.name = name;
      while (se.name.length() < MAX_NAME_LENGTH) se.name +=' ';
      se.value = value;
      if (se.value.length() < MAX_SCORE_LENGTH) se.value.insert(0, MAX_SCORE_LENGTH - se.value.length(), ' ');
      scores.push_back(se);
      addText(glm::vec2(0.0, 0.6f - (count/10.0f) * 1.5 ), entry_place[count] + se.name + " " + se.value, glm::vec2(0.14 - count/100.0f));
      
      count++;
   }
}

void ScoreState::addText(glm::vec2 topLeft, std::string message, glm::vec2 font_size = glm::vec2(0.16f)) {
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
      opacities.push_back(1.0f);
      opacities.push_back(1.0f);
      
      topLeft.x += font_spacing;
   }
}

//void ScoreState::printScores() {
//   std::vector<ScoreEntry>::iterator iterator = scores.begin();
//   int counter = 0;
//   while(iterator < scores.end()) {
//      ScoreEntry se = *iterator;
//      std::cout << "Index " << counter++ << " | " << se.name << ":" << se.value << std::endl;
//      iterator++;
//   }
//}

void ScoreState::update(float dt) {
   //TODO:
   // get user keyboard inputs to construct string
   // check for ENTER key to move on to different state?
   // also write to the txt file to save scores
   // also remove > 5 inputs 
   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
   helper->bufferData(Opacities, opacities);
}

void ScoreState::render(float dt) {
   game_state->render(dt);
   
   helper->render(glm::mat4(1));
}