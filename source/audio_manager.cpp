#include "audio_manager.h"

#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

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

      fprintf(stderr, "FMOD %s while doing %s: %s\n", level.c_str(), message.c_str(), FMOD_ErrorString(result));
   }

   return result == FMOD_OK;
}

} // namespace

AudioManager::AudioManager()
   : system(nullptr), musicGroup(nullptr) {
}

AudioManager::~AudioManager() {
   release();
}

void AudioManager::release() {
   if (system) {
      FMOD_RESULT result = system->release();
      system = nullptr;

      if (result != FMOD_OK) {
         fprintf(stderr, "Unable to release FMOD system\n");
      }
   }
}

bool AudioManager::load(const std::string &fileName) {
   if (!system) {
      fprintf(stderr, "Audio system not initialized\n");
      return false;
   }

   FMOD_MODE mode = FMOD_DEFAULT | FMOD_LOOP_NORMAL;
   FMOD::Sound *sound;
   if (!check(system->createStream(fileName.c_str(), mode, nullptr, &sound), "loading sound", ErrorLevel::Error)) {
      return false;
   }

   soundMap[fileName] = sound;
   return true;
}

void AudioManager::init() {
   if (system) {
      fprintf(stderr, "Audio system not initialized\n");
      return;
   }

   if (!check(FMOD::System_Create(&system), "audio system creation", ErrorLevel::Fatal)) {
      release();
      return;
   }

   unsigned int version;
   if (!check(system->getVersion(&version), "version check", ErrorLevel::Fatal)) {
      return;
   }

   if (version != FMOD_VERSION) {
      fprintf(stderr, "FMOD lib version %u doesn't match header version %u\n", version, FMOD_VERSION);
      return;
   }

   //check(FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_WARNING, FMOD_DEBUG_MODE_CALLBACK, nullptr));

   if (!check(system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr), "audio system initialization", ErrorLevel::Fatal)) {
      release();
      return;
   }

   system->createChannelGroup("music", &musicGroup);
}

void AudioManager::update() {
   if (system) {
      fprintf(stderr, "Audio system not initialized\n");
      return;
   }

   check(system->update(), "audio system update");
}

void AudioManager::play(const std::string &fileName) {
   if (system) {
      fprintf(stderr, "Audio system not initialized\n");
      return;
   }

   if (!soundMap.count(fileName)) {
      fprintf(stderr, "No sound effect with file name: %s\n", fileName.c_str());
      return;
   }

   FMOD::Channel *channel = nullptr;
   check(system->playSound(soundMap.at(fileName), musicGroup, true, &channel), "sound play");

   if (!channel) {
      fprintf(stderr, "Unable to play sound: %s\n", fileName.c_str());
      return;
   }

   check(channel->setLoopCount(-1), "set infinite loop");

   check(channel->setPaused(false), "sound unpause");
}
