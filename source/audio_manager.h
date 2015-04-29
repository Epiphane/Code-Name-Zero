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
   
public:
   Music(FMOD::Sound *s, Beat _bpm) : channel(nullptr), sound(s), beat(0), bpm(_bpm) {};
   
   void update();
   void play();
   void start();
   void pause();
   
   FMOD::Channel *getChannel() { return channel; }
   FMOD::Sound *getSound() { return sound; }
   unsigned int getBeat()  { return beat; }
   
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
