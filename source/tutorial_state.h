//
//  tutorial_state.h
//  Project
//
//  Created by Thomas Steinke on 5/30/15.
//
//

#ifndef __Project__tutorial_state__
#define __Project__tutorial_state__

#include "rendererText.h"
#include "in_game_state.h"

class TutorialState : public InGameState {
private:
   class TextEvent {
   private:
      bool done;
      int buffer_index, length;

      float keyframes[4];
      float fadeIn, fadeOut;

   public:
      TextEvent(Beat in, Beat out, float fadeIn, float fadeOut);
      void setStringInfo(unsigned int ndx, int len) { buffer_index = ndx; length = len; }

      void update(float current_beat, TutorialState *state);

      bool isDone() { return done; }
   };

   std::vector<TextEvent *> events;

   RendererText *helper;
   void addTextEvent(TextEvent *e, glm::vec2 topLeft, std::string message);

public:
   TutorialState(int playership);

   void update(float dt);
   void render(float dt);

   void send(std::string message, void *data);
};

#endif /* defined(__Project__tutorial_state__) */
