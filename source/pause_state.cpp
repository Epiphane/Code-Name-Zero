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
   helper = new Renderer2D("./textures/pause_screen.png", true, 0.0f);
   renderPauseScreen();
   
   game_state = game;
   initializeVariables();
   
   input_clear();
   input_set_callback(GLFW_KEY_1, resumeGame);
   input_set_callback(GLFW_KEY_2, goToMainMenu);
}

void PauseState::initializeVariables() {
   resume = false;
   main_menu = false;
}

void PauseState::renderPauseScreen() {
   std::vector<glm::vec2> positions, uvs;
   positions.push_back(glm::vec2(-0.5, 0.7));
   positions.push_back(glm::vec2(0.5, -0.2));
   uvs.push_back(glm::vec2(0));
   uvs.push_back(glm::vec2(1));
   
   std::vector<float> opacities;
   opacities.push_back(1);
   opacities.push_back(1);
   
   helper->bufferData(Opacities, opacities);
   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
}

void PauseState::update(float dt) {
   if (resume) {
      input_clear();
      togglePauseDebug();
      setState(game_state);
   } else if (main_menu) {
      initializeVariables();
      input_clear();
      togglePauseDebug();
      setState(new ShipSelect());
   }
}

void PauseState::render(float dt) {
   game_state->render(dt);
   
   helper->render(glm::mat4(1.0f));
}