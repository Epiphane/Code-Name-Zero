//
//  audio_visualizer.h
//  RGBZero
//
//  Created by Thomas on 5/17/15.
//
//

#ifndef __RGBZero__audio_visualizer__
#define __RGBZero__audio_visualizer__

#include <stdio.h>

#include "GLSL.h"
#include "vertex_buffer_object.h"
#include "audio_manager.h"

#include "renderer2D.h"

class AudioVisualizer {
private:
   GLuint program;
   
   Music *music;
   Renderer2D *renderer;
   
   void init();
   
public:
   AudioVisualizer(Music *music);
   
   void update(float dt);
   void render();
};

#endif /* defined(__RGBZero__audio_visualizer__) */
