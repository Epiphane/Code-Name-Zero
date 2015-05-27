//
//  audio_visualizer.cpp
//  RGBZero
//
//  Created by Thomas on 5/17/15.
//
//

#include <FMOD/fmod.hpp>
#include "shader.hpp"
#include "renderer.h"
#include "audio_visualizer.h"
#include "input_manager.h"

void do_the_thing();

void AudioVisualizer::init() {
   // Only initialize the shader program once
   static bool initialized = false;
   if (initialized)
      return;
   
   program = LoadShaders("./shaders/AudioVisualizerVertex.glsl", "./shaders/AudioVisualizerGeometry.glsl", "./shaders/AudioVisualizerFragment.glsl");
   
   input_set_callback(GLFW_KEY_O, do_the_thing);
   
   initialized = true;
}

AudioVisualizer::AudioVisualizer(Music *m) : music(m) {
   init();
   
   renderer = new Renderer2D("./textures/progress.png", 1.499999);
}

int num_bars = Music::MAX_SPECTRA;
void do_the_thing() {
   num_bars /= 2;
   if (num_bars == 0) num_bars = Music::MAX_SPECTRA;
}

void AudioVisualizer::update(float dt) {
   float samples[Music::MAX_SPECTRA] = {0};
   music->getSamples(samples, num_bars);
   
   float MAX_VOL = 0.3;
   for (int i = 0; i < num_bars; i ++)
      if (samples[i] > MAX_VOL) MAX_VOL = samples[i];
   
   std::vector<glm::vec2> vertices, uvs;
   
   float x = -0.64, y = 0;
   float w = (x * -2) / num_bars;
   float h = 0.5;
   for (int bar = 0; bar < num_bars; bar ++) {
      if (samples[bar] < 0.001) samples[bar] = 0.001;
      
      vertices.push_back(glm::vec2(x, y));
      vertices.push_back(glm::vec2(x + w, y + h * samples[bar] / MAX_VOL));
      
      uvs.push_back(glm::vec2(1));
      uvs.push_back(glm::vec2(0, 1 - samples[bar] / MAX_VOL));
      
      x += w;
   }
   
   float sounds[3];
   sounds[0] = music->getLow ();
   sounds[1] = music->getMid ();
   sounds[2] = music->getHigh();
   for (int i = 0; i < 3; i ++) {
      vertices.push_back(glm::vec2(-1 + i * 0.1,     0));
      vertices.push_back(glm::vec2(-1 + (i+1) * 0.1, 0 + sounds[i]));
      
      uvs.push_back(glm::vec2(1));
      uvs.push_back(glm::vec2(0, 1 - sounds[i]));
   }
   
   renderer->bufferData(Vertices, vertices);
   renderer->bufferData(UVs, uvs);
}

void AudioVisualizer::render() {
   renderer->render(glm::mat4(1.0f));
}