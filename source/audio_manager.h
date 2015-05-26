//
//  audio_manager.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/21/15.
//
//

#ifndef __RGBZero__audio_manager__
#define __RGBZero__audio_manager__

#include <string>

namespace FMOD {
   class Channel;
   class ChannelGroup;
   class Sound;
   class System;
   class DSP;
} // namespace FMOD

typedef unsigned int Beat;
typedef void(* beat_callback)(Beat);

class Music {
public:
   const static int MAX_SPECTRA = 128;
   const static int SAMPLES_PER_SPECTRUM = 256;
   
private:
   FMOD::Channel *channel;
   FMOD::Sound *sound;
   FMOD::DSP *dsp;
   Beat beat;
   Beat bpm;
   unsigned int length;
   
   int currentSpectrum = 0;
   float spectrum[MAX_SPECTRA][SAMPLES_PER_SPECTRUM] = {{0}};
   
public:
   Music(FMOD::Sound *s, Beat _bpm);
   void setChannel(FMOD::Channel *c) { channel = c; }
   
   FMOD::Channel *getChannel() { return channel; }
   FMOD::Sound *getSound() { return sound; }
   FMOD::DSP *getDSP() { return dsp; }
   
   /* 
    * Get the nth sample out of an index size outOf
    * outOf must be a power of 2
    *
    * Maximum: MAX_SPECTRA
    * Minimum: 1 (for volume, perhaps)
    */
   float getSample(int index, int outOf);
   float getLow()  { return getSample(0, 4); }
   float getMid()  { return getSample(2, 4); }
   float getHigh() { return getSample(1, 4); }
   // Or just put all spectra in an array
   void getSamples(float *samples, int number);
   
   void update();
   void play();
   
   Beat getBeat()  { return beat; }
   Beat getBPM() { return bpm; }
   unsigned int getLength() { return length; }
   float getProgress();
};

void audio_init();
void audio_setPaused(bool paused);
void audio_update();
void audio_release();

FMOD::Sound *audio_load_sound(const char *filename);
void audio_play_sound(const char *filename);

Music *audio_load_music(const char *fileName, Beat bpm);
void audio_play_music(Music *music);

void audio_stuff();

#endif /* defined(__RGBZero__audio_manager__) */
