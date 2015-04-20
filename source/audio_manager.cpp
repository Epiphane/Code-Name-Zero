#include <iostream>
#include <cassert>
#include <vector>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

#include "main.h"
#include "state.h"
#include "audio_manager.h"

namespace {
   const int MAX_CHANNELS = 64;
   
   enum class ErrorLevel {
      Warning,
      Error,
      Fatal
   };
   
   bool check(FMOD_RESULT result, const std::string &message, ErrorLevel errorLevel = ErrorLevel::Warning) {
      if (result != FMOD_OK) {
         std::string level;
         switch (errorLevel) {
            case ErrorLevel::Warning:
               level = "warning";
               break;
            case ErrorLevel::Error:
               level = "error";
               break;
            case ErrorLevel::Fatal:
               level = "fatal";
               break;
            default:
               level = "invalid";
         }
         
         std::cerr << "FMOD " << level << " while doing " << message << ": " <<FMOD_ErrorString(result) << std::endl;
      }
      
      return result == FMOD_OK;
   }
} // namespace

FMOD::System *audio_system = nullptr;
FMOD::ChannelGroup *sounds = nullptr;
std::vector<Music *> musics;
std::unordered_map<std::string, FMOD::Sound*> soundMap;

void audio_init() {
   soundMap.clear();
   
   assert(audio_system == nullptr);
   
   if (!check(FMOD::System_Create(&audio_system), "system creation", ErrorLevel::Fatal)) {
      audio_release();
      return;
   }
   
   unsigned int version;
   if (!check(audio_system->getVersion(&version), "version check", ErrorLevel::Fatal)) {
      return;
   }
   
   if (version != FMOD_VERSION) {
      std::cerr << "FMOD lib version " << version << " doesn't match header version " << FMOD_VERSION << std::endl;
      return;
   }
   
   if (!check(audio_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr), "audio system initialization", ErrorLevel::Fatal)) {
      audio_release();
      return;
   }
   
   check(audio_system->createChannelGroup("music", &sounds), "channel group creation");
   check(sounds->setPaused(DEBUG), "sound pause setup");
}

void audio_setPaused(bool paused) {
   assert(audio_system != nullptr);
   assert(sounds != nullptr);
   
   sounds->setPaused(paused);
}

void audio_update() {
   assert(audio_system != nullptr);
   check(audio_system->update(), "audio system update");
   
   std::vector<Music *>::iterator it;
   for(it = musics.begin(); it < musics.end(); it ++) {
      (*it)->update();
   }
}

void audio_release() {
   assert(audio_system == nullptr);
   
   sounds->stop();
   sounds->release();
}

FMOD::Sound *audio_load_sound(const char *fileName) {
   assert(audio_system != nullptr);
   
   if (soundMap[fileName])
      return soundMap[fileName];
   
   FMOD::Sound *sound;
   if (!check(audio_system->createStream(fileName, FMOD_DEFAULT, nullptr, &sound), "loading sound", ErrorLevel::Error)) {
      return nullptr;
   }
   
   return sound;
}

Music *audio_load_music(const char *fileName, Beat bpm) {
   FMOD::Sound *sound = audio_load_sound(fileName);
   check(sound->setMode(FMOD_DEFAULT | FMOD_LOOP_NORMAL), "sound looping", ErrorLevel::Error);
   check(sound->setLoopCount(-1), "sound->setLoopCount(-1)", ErrorLevel::Error);
   
   Music *music = new Music(sound, bpm);
   musics.push_back(music);
   return music;
}

void audio_play_music(Music *music) {
   assert(audio_system != nullptr);

   if (!music->getSound()) {
      std::cerr << "No sound registered to this music" << std::endl;
      return;
   }
   
   // Start music if it exists already
   FMOD::Channel *channel = music->getChannel();
   if (!channel) {
      check(audio_system->playSound(music->getSound(), sounds, true, &channel), "music play");
      
      if (!channel) {
         std::cerr << "Unable to play music" << std::endl;
         return;
      }
   }
   
   check(channel->setPaused(false), "sound unpause");
   music->setChannel(channel);
}

void audio_play_sound(const char *fileName) {
   assert(audio_system != nullptr);
   
   FMOD::Sound *sound = audio_load_sound(fileName);
   assert(sound != nullptr); // Sound file may not exist
   
   FMOD::Channel *channel;
   check(audio_system->playSound(sound, sounds, true, &channel), "sound play");
   
   channel->setPaused(false);
}

void Music::update() {
   Beat position;
   channel->getPosition(&position, FMOD_TIMEUNIT_MS);
   
   Beat currentBeat = position * bpm / 60000 + 1;
   if (currentBeat != beat) {
      beat = currentBeat;
      
      getCurrentState()->send("beat", &beat);
   }
}

void Music::play() {
   audio_play_music(this);
}
