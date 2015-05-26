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
} // namespace FMOD

typedef unsigned int Beat;
typedef void(* beat_callback)(Beat);

class Music {
private:
   FMOD::Channel *channel;
   FMOD::Sound *sound;
   Beat beat;
   Beat bpm;
   unsigned int length;
   
public:
   Music(FMOD::Sound *s, Beat _bpm);
   
   void update();
   void play();
   
   unsigned int getLength() { return length; }
   float getProgress();
   
   FMOD::Channel *getChannel() { return channel; }
   FMOD::Sound *getSound() { return sound; }
   Beat getBeat()  { return beat; }
   Beat getBPM() { return bpm; }
   
   void setChannel(FMOD::Channel *c) { channel = c; }
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
