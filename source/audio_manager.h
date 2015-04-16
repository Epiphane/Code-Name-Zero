#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <unordered_map>

namespace FMOD {
   class Channel;
   class ChannelGroup;
   class Sound;
   class System;
} // namespace FMOD

class Music {
private:
   FMOD::Channel *channel;
   FMOD::Sound *sound;
   unsigned int beat;
   
public:
   Music(FMOD::Sound *s) : channel(nullptr), sound(s), beat(0) {};
   
   void update();
   void play();
   
   FMOD::Channel *getChannel() { return channel; }
   FMOD::Sound *getSound() { return sound; }
   unsigned int getBeat()  { return beat; }
   
   void setChannel(FMOD::Channel *c) { channel = c; }
};

void audio_init();
void audio_update();
void audio_release();

FMOD::Sound *audio_load_sound(const char *filename);
void audio_play_sound(const char *filename);

Music *audio_load_music(const char *fileName);
void audio_play_music(Music *music);

#endif
