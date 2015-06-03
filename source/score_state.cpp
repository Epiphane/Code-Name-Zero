//
//  score_state.cpp
//  Project
//
//  Created by Jonathan Pae on 6/02/15.
//
//

#include "score_state.h"

ScoreState::ScoreState(State *game) {
   game_state = game;
   
   helper = new Renderer2D("./textures/speed_font.png", true, 0);
   
   positions.clear();
   uvs.clear();
   
   //Display top scores from scores.txt
   
}


void ScoreState::update(float dt) {
   
}

void ScoreState::render(float dt) {
   game_state->render(dt);
   
   helper->render(glm::mat4(1));
}

void send(std::string message, void *data) {
   
}