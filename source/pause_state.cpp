#include "pause_state.h"
#include "tutorial_state.h"
#include "ship_select_state.h"
#include "main.h"
#include "camera.h"

void togglePauseDebug() {
   PAUSED = !PAUSED;
   
   audio_setPaused(PAUSED);
   camera_setDebug(PAUSED);
}

void resumeGame();
void goToMainMenu();

bool resume = false;
bool main_menu = false;

void resumeGame() {
   resume = true;
}

void goToMainMenu() {
   main_menu = true;
}

PauseState::PauseState(State *game) {
   helper = new RendererText();//RendererText::instance();
   
   //Save any current text in the screen
   helper->saveBuffers();
   helper->clearAllText();
   
   game_state = game;
   initializeVariables();
   
   input_clear();
   input_set_callback(GLFW_KEY_1, resumeGame);
   input_set_callback(GLFW_KEY_2, goToMainMenu);
   
   helper->addText(glm::vec2(0.0, 0.60), "PAUSED", glm::vec2(0.15));
   helper->addText(glm::vec2(0.0, -0.0), "1. Resume this level", glm::vec2(0.12));
   helper->addText(glm::vec2(0.0, -0.1), "2. Go back to main menu", glm::vec2(0.12));
}

void PauseState::initializeVariables() {
   resume = false;
   main_menu = false;
}

void PauseState::update(float dt) {
   if (resume) {
      helper->reloadBuffers();
      input_clear();
      togglePauseDebug();
      setState(game_state);
   } else if (main_menu) {
      helper->clearAllText();
      initializeVariables();
      input_clear();
      togglePauseDebug();
      setState(new ShipSelect());
   }
}

void PauseState::render(float dt) {
   game_state->render(dt);
   
   helper->render();
}