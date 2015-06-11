//
//  tutorial_state.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 5/30/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include "tutorial_state.h"
#include "score_state.h"

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
      state->helper->opacities[i] = opacity;
}

TutorialState::TutorialState(int playership, LevelInfo *levelinfo) : InGameState("RGB_Tutorial", 200, playership, levelinfo) {
   helper = new RendererText();

   events.clear();
   helper->clearAllText();

   addTextEvent(new TextEvent(8, 18, 4, 8), glm::vec2(0, 0.5f), "Hello, Commander.");
   addTextEvent(new TextEvent(16, 18, 8, 8), glm::vec2(0, 0.25f), "Welcome to RGB-Zero.");

   addTextEvent(new TextEvent(30, 36, 4, 4), glm::vec2(0, 0.5f), "The Goal is simple.");
   addTextEvent(new TextEvent(34, 38, 2, 2), glm::vec2(0, 0.25f), "GO FAST.");

   addTextEvent(new TextEvent(44, 50, 4, 2), glm::vec2(0, 0.5f), "Match colors with");
   addTextEvent(new TextEvent(44, 50, 4, 2), glm::vec2(0, 0.25f), "obstacles to speed up.");

   addTextEvent(new TextEvent(54, 72, 2, 4), glm::vec2(0, 0.5f), "Lets try it now");
   addTextEvent(new TextEvent(54, 72, 2, 4), glm::vec2(0, 0.25f), "Press \5");

   helper->updateBuffers();
}

void TutorialState::send(std::string message, void *data) {
   InGameState::send(message, data);
   if (message == "beat") {
      Beat current_beat = *(Beat *)data;
      static int playerStartSpeed = 0;
      if (current_beat == 72) {
         playerStartSpeed = getPlayerSpeed();
      }
      else if (current_beat == 80) {
         // Check speed, make sure they did it right
         if (getPlayerSpeed() - playerStartSpeed < 5) {
            getSoundtrack()->rewind(48);
            addTextEvent(new TextEvent(44, 72, 2, 4), glm::vec2(0, 0.5f), "Lets try that again");
            addTextEvent(new TextEvent(44, 72, 2, 4), glm::vec2(0, 0.25f), "Press \5");
         }
         else {
            addTextEvent(new TextEvent(86, 102, 4, 4), glm::vec2(0, 0.5f), "Good!");
            
            addTextEvent(new TextEvent(90, 106, 4, 4), glm::vec2(0, 0.25f), "You can also use");
            addTextEvent(new TextEvent(90, 106, 4, 4), glm::vec2(0, 0), "\6 to move right");
            
            addTextEvent(new TextEvent(114, 120, 4, 4), glm::vec2(0, 0.5f), "Or \2 \3 \4");
            addTextEvent(new TextEvent(114, 120, 4, 4), glm::vec2(0, 0.25f), "For each lane");

            addTextEvent(new TextEvent(140, 144, 4, 4), glm::vec2(0, 0.5f), "SHALL WE?");
         }
      }
   }
}

void TutorialState::addTextEvent(TutorialState::TextEvent *e, glm::vec2 topLeft, std::string message) {
   e->setStringInfo(helper->positions.size(), message.length());
   events.push_back(e);

   helper->addText(topLeft, message, glm::vec2(0.16f) ,0.0f);
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
   helper->updateBuffers();
}

void TutorialState::render(float dt) {
   InGameState::render(dt);

   if (!PAUSED)
   helper->render();
}