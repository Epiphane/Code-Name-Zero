//
//  score_state.cpp
//  Project
//
//  Created by Jonathan Pae on 6/02/15.
//
//

#include "score_state.h"
#include "tutorial_state.h"
#include "main.h"
#include "ship_select_state.h"

#include <string>
#include <fstream>


const char *entry_place[] = { "1st: ", "2nd: ", "3rd: ", "4th: ", "5th: " };
ScoreEntry new_entry;
bool madeHighScore = true;
bool nextScreen = false;
bool retryLevel = false;
bool toMainMenu = false;
int player_place = 0;
std::string lastRenderedName;

int playerShipIndex;

void deleteCharacter();
void updateString(char ch);
void enterKeyPressed();
void retry();
void mainMenu();

ScoreState::ScoreState(State *game) {
   std::string name;
   std::string value;
   int count = 0;
   
   
   initializeVariables();
   
   game_state = game;
   
   helper = new Renderer2D("./textures/speed_font.png", true, 0);
   
   addText(glm::vec2(0.0, 0.75f), "HIGH SCORES", glm::vec2(0.15));
   //Retrieve top scores from scores.txt
   std::ifstream infile("./scores.txt");
   while (infile >> name >> value) {
      ScoreEntry se;
      se.name = name;
      while (se.name.length() < MAX_NAME_LENGTH) se.name +=' ';
      se.value = value;
      if (se.value.length() < MAX_SCORE_LENGTH) se.value.insert(0, MAX_SCORE_LENGTH - se.value.length(), ' ');
      scores.push_back(se);
      addText(glm::vec2(0.0, 0.6f - (count/10.0f) * 1.3 ), entry_place[count] + se.name + " " + se.value, glm::vec2(0.14 - count/100.0f));
      
      count++;
   }
   
   // Retrieve the player's score
   InGameState *s = dynamic_cast<InGameState *>(game_state);
   long player_score = s->getScore();
   playerShipIndex = s->getShipIndex();
   
   // find the player's placement among the scorers
   std::vector<ScoreEntry>::iterator it = scores.begin();
   while (it < scores.end()) {
      ScoreEntry se = *it;
      long entry_value = std::atol(se.value.c_str());
      if (entry_value < player_score) break;
      it ++;
      player_place ++;
   }
   
   new_entry.value = std::to_string(player_score);
   srand(atoi(new_entry.value.c_str()));
   if (player_place >= MAX_SCORE_ENTRY) {
      madeHighScore = false;
   }
   
   if (madeHighScore) {
      input_on_alphaKey(updateString);
      input_set_callback(GLFW_KEY_BACKSPACE, deleteCharacter);
      addText(glm::vec2(0.0, -0.1), "Congratulations!", glm::vec2(0.12));
      addText(glm::vec2(0.0, -0.2), "You made top 5", glm::vec2(0.12));
      addText(glm::vec2(-0.4, -0.4), "Name: ", glm::vec2(0.12));
      addText(glm::vec2(0.3, -0.4), new_entry.name, glm::vec2(0.13));
      addText(glm::vec2(0.0, -0.55), "Press ENTER to continue", glm::vec2(0.12));
   } else {
      //Display some message
      addText(glm::vec2(0.0, -0.1), "Looks like you need more practice!", glm::vec2(0.12));
      addText(glm::vec2(0.0, -0.2), "Press ENTER to continue", glm::vec2(0.12));
   }
   input_set_callback(GLFW_KEY_ENTER, enterKeyPressed);
}

void ScoreState::regenFrameBuffers() {
   game_state->regenFrameBuffers();
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

void ScoreState::removeLastText() {
   if (positions.size() > 0 && uvs.size() > 0 && opacities.size() > 0) {
      positions.pop_back();
      positions.pop_back();
      uvs.pop_back();
      uvs.pop_back();
      opacities.pop_back();
      opacities.pop_back();
   }
}

void ScoreState::clearAllText() {
   positions.clear();
   uvs.clear();
   opacities.clear();
}

void updateString(char ch) {
   if (new_entry.name.length() < MAX_NAME_LENGTH) {
      new_entry.name += ch;
   }
}

void deleteCharacter() {
   if (new_entry.name.length() > 0) {
      new_entry.name.pop_back();
   }
}

void enterKeyPressed() {
   nextScreen = true;
}

void retry() {
   retryLevel = true;
}

void mainMenu() {
   toMainMenu = true;
}

void ScoreState::saveToFile() {
   // only want to save top 5
   while (scores.size() > MAX_SCORE_ENTRY) {
      scores.pop_back();
   }
   
   ofstream newFile("./scores.txt");
   
   if(newFile.is_open())
   {
      for (auto entry : scores) // access by reference to avoid copying
      {
         newFile << entry.name + " " + entry.value << endl;
      }
   }
   else {
      // Please create a scores/ folder!!!
      assert(0);
   }
   newFile.close();
}

void ScoreState::initializeVariables() {
   //class variables
   scores.clear();
   
   
   //file variables
   madeHighScore = true;
   nextScreen = false;
   retryLevel = false;
   toMainMenu = false;
   player_place = 0;
   lastRenderedName = "";
   new_entry.value = "";
   new_entry.name = "";
   
   clearAllText();
   input_clear();
}

void ScoreState::update(float dt) {
   //game_state->update(dt);
   
   if (nextScreen) {
      input_clear();
      
      input_set_callback(GLFW_KEY_1, retry);
      input_set_callback(GLFW_KEY_2, mainMenu);
      
      if (madeHighScore) {
         // add ScoreEntry to scores
         scores.insert(scores.begin() + player_place, new_entry);
         saveToFile();
         madeHighScore = false;
      }
      clearAllText();
      
      //Create new menu for the user
      addText(glm::vec2(0.0, 0.2), "What would you like to do now?", glm::vec2(0.12));
      addText(glm::vec2(0.0, -0.0), "1. Retry this level", glm::vec2(0.12));
      addText(glm::vec2(0.0, -0.1), "2. Go back to main menu", glm::vec2(0.12));
      
      nextScreen = false;
   } else if (toMainMenu) {
      //TODO: set state to the actual main menu
      initializeVariables();
      setState(new ShipSelect());
   } else if (retryLevel) {
      //TODO: set state to appropriate "level" it won't aways be tutorial
      initializeVariables();
      setState(new TutorialState(playerShipIndex));
   } else if (madeHighScore) {
      for (int i = 0; i < lastRenderedName.length(); i++) removeLastText();
      
      addText(glm::vec2(0.0, -0.4), new_entry.name, glm::vec2(0.13));
      lastRenderedName = new_entry.name;
   }
   
   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
   helper->bufferData(Opacities, opacities);
}

void ScoreState::render(float dt) {
   game_state->render(dt);
   
   helper->render(glm::mat4(1));
}