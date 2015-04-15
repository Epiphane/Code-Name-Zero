#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <unordered_map>

namespace FMOD {

class ChannelGroup;
class Sound;
class System;

} // namespace FMOD

typedef std::unordered_map<std::string, FMOD::Sound*> SoundMap;

class AudioManager {
protected:
   FMOD::System *system;
   FMOD::ChannelGroup *musicGroup;

   SoundMap soundMap;

   void release();

   bool load(const std::string &sound);

public:
   AudioManager();

   virtual ~AudioManager();

   void init();

   void update();

   void play(const std::string &sound);
};

#endif
