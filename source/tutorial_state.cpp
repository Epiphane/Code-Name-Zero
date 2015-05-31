//
//  tutorial_state.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 5/30/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include "tutorial_state.h"

TutorialState::TextEvent::TextEvent(Beat in, Beat out, float fadeIn, float fadeOut) : done(false), buffer_index(-1) {
   keyframes[0] = in - fadeIn;
   keyframes[1] = in;
   keyframes[2] = out;
   keyframes[3] = out + fadeOut;

   this->fadeIn = fadeIn;
   this->fadeOut = fadeOut;
}

void TutorialState::TextEvent::update(float current_beat, TutorialState *state) {
   assert(buffer_index >= 0);
   
   // Check "done"ness
   if (done) return;

   // Check whether we even start yet
   if (keyframes[0] > current_beat) {
      return;
   }

   // Update opacity of text
   float opacity = 1;
   if (current_beat < keyframes[1]) {
      // Fade in
      opacity = (current_beat - keyframes[0]) / fadeIn;
   }
   else {
      // Fade out
      opacity = (keyframes[3] - current_beat) / fadeOut;
   }
   
   // Done animating?
   if (keyframes[3] < current_beat) {
      opacity = 0;
      done = true;
   }

   for (int i = buffer_index; i < buffer_index + 2 * length; i++)
      state->opacities[i] = opacity;
}

TutorialState::TutorialState(int playership) : InGameState("RGB_Tutorial", 100, playership) {
   helper = new Renderer2D("./textures/speed_font.png", true, 0);

   events.clear();
   positions.clear();
   uvs.clear();
   opacities.clear();

   addTextEvent(new TextEvent(8, 14, 4, 4), glm::vec2(0, 0.5f), "Hello, Commander.");
   addTextEvent(new TextEvent(12, 14, 4, 4), glm::vec2(0, 0.25f), "Welcome to RGB-Zero.");

   addTextEvent(new TextEvent(20, 24, 2, 2), glm::vec2(0, 0.5f), "The Goal is simple.");
   addTextEvent(new TextEvent(22, 25, 1, 1), glm::vec2(0, 0.25f), "GO FAST.");

   addTextEvent(new TextEvent(28, 32, 2, 2), glm::vec2(0, 0.5f), "Match colors with");
   addTextEvent(new TextEvent(28, 32, 2, 2), glm::vec2(0, 0.25f), "obstacles to speed up.");

   addTextEvent(new TextEvent(36, 40, 2, 2), glm::vec2(0, 0.5f), "Lets try it now");
   addTextEvent(new TextEvent(36, 40, 2, 2), glm::vec2(0, 0.25f), "Press \2");

   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
   helper->bufferData(Opacities, opacities);
}

void TutorialState::send(std::string message, void *data) {
   InGameState::send(message, data);
   if (message == "beat") {
      Beat current_beat = *(Beat *)data;
      static int playerStartSpeed = 0;
      if (current_beat == 36) {
         playerStartSpeed = getPlayerSpeed();
      }
      else if (current_beat == 44) {
         // Check speed, make sure they did it right
         if (getPlayerSpeed() - playerStartSpeed < 45) {
            getSoundtrack()->rewind(24);
            addTextEvent(new TextEvent(28, 40, 2, 2), glm::vec2(0, 0.5f), "Lets try that again");
            addTextEvent(new TextEvent(28, 40, 2, 2), glm::vec2(0, 0.25f), "Press \2");
         }
         else {
            addTextEvent(new TextEvent(48, 52, 2, 2), glm::vec2(0, 0.5f), "Good!");
            addTextEvent(new TextEvent(50, 54, 2, 2), glm::vec2(0, 0.25f), "\3 and \4 correspond");
            addTextEvent(new TextEvent(50, 54, 2, 2), glm::vec2(0, 0), "To green and red.");

            addTextEvent(new TextEvent(58, 64, 2, 2), glm::vec2(0, 0.5f), "You can also use");
            addTextEvent(new TextEvent(58, 64, 2, 2), glm::vec2(0, 0.25f), "A and D to move sideways");

            addTextEvent(new TextEvent(68, 72, 2, 2), glm::vec2(0, 0.5f), "SHALL WE?");
         }
      }
   }
}

void TutorialState::addTextEvent(TutorialState::TextEvent *e, glm::vec2 topLeft, std::string message) {
   e->setStringInfo(positions.size(), message.length());
   events.push_back(e);

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
      opacities.push_back(0);
      opacities.push_back(0);

      topLeft.x += font_spacing;
   }
}

void TutorialState::update(float dt) {
   InGameState::update(dt);

   float current_beat = getSoundtrack()->getBeat() + float(getSoundtrack()->getBeatOffsetMS()) / 1000;
   std::vector<TextEvent *>::iterator it = events.begin();
   while (it < events.end()) {
      (*it)->update(current_beat, this);
      if ((*it)->isDone())
         it = events.erase(it);
      else
         it++;
   }
   helper->bufferData(Vertices, positions);
   helper->bufferData(UVs, uvs);
   helper->bufferData(Opacities, opacities);
}

void TutorialState::render(float dt) {
   InGameState::render(dt);

   helper->render(glm::mat4(1));
}