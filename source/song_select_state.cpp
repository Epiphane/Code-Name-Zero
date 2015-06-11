//  song_select_state.cpp
//  RGBZero
//
//  Created by David Ellison on 6/7/15.
//
//

#include "song_select_state.h"
#include "tutorial_state.h"
#include "main.h"
#include "ship_manager.h"
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

//constants
#define AUDIO_DIR "audio/"
#define AUDIO_EXT ".mp3"
#define BEATMAP_DIR "beatmaps/"
#define BEATMAP_EXT ".beatmap"

//sort of a constant
#define HIGHEST_BPM 200

//input callbacks--need ship here because
//the functions need to be static
int shipIndex;
int currentSong = 0;
LevelInfo *currentLevel;
int MAX_SONGS;

//todo move to class declaration
ShipManager *shipManager;
shared_ptr<GameObject> currentShip;

void nextState();
void scrollDown();
void scrollUp();

SongSelect::SongSelect(int shipndx) : StaticState("level_select_screen", audio_load_music("./audio/RGBZeroMenu.mp3", 135, true)) {
   shipIndex = shipndx;
   
   helper = RendererText::instance();
   helper->clearAllText();
   ratingsRenderer = RendererRatings::instance();
   ratingsRenderer->clearAllRatings();
   
   shipManager = ShipManager::instance();
   
   initializeList();
}

void SongSelect::start() {
   input_set_callback(GLFW_KEY_SPACE, nextState);
   input_set_callback(GLFW_KEY_A, scrollUp);
   input_set_callback(GLFW_KEY_D, scrollDown);
   
   currentShip = std::make_shared<GameObject>(shipManager->getModel(shipIndex)->getModelRenderer());
   addObject(currentShip);
}

//read in song names and metadata
void SongSelect::initializeList() {
   std::ifstream beatmaps;
   std::string line;
   std::vector<std::string> filenames;
   std::string songTitle;
   int songBpm;
   
   //find songs from songs.txt
   std::ifstream songListStream("./songs.txt");
   while (std::getline(songListStream, line)) {
      filenames.push_back(line);
   }
   
   for (int i = 0; i < filenames.size(); i++) {
      //if the beatmap exists, create a LevelData from it
      beatmaps = ifstream(BEATMAP_DIR + filenames[i] + BEATMAP_EXT);
      
      if (beatmaps.good()) {
         std::getline(beatmaps, songTitle);
         beatmaps >> songBpm;
         allSongs.push_back(new LevelInfo(filenames[i],
                                          songTitle,
                                          songBpm,
                                          true));
      } else {
         //TODO figure out wtf to do if there's no beatmap
      }
   }
   MAX_SONGS = allSongs.size();
}

void SongSelect::render(float dt) {
   this->StaticState::render(dt);
   helper->render();
   //ratingsRenderer->render();
}

void SongSelect::update(float dt) {
   static float currentShipFloatAmt = 0.0f;
   static bool floatingUp = true;
   
   helper->clearAllText();
   ratingsRenderer->clearAllRatings();
   
   
   currentShip->clearModel();
   
   if (currentShipFloatAmt >= 0.15) {
      floatingUp = false;
   } else if (currentShipFloatAmt <= -0.15) {
      floatingUp = true;
   }
   
   if (floatingUp) {
      currentShipFloatAmt += 0.2 * dt;
   } else {
      currentShipFloatAmt -= 0.2 * dt;
   }
   
   currentShip->transform(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -0.5f + currentShipFloatAmt, -1.0f)));
   
   helper->addText(glm::vec2(0.0, 0.3), allSongs[currentSong]->songTitle, glm::vec2(0.075), 1.0f);
   
   helper->updateBuffers();
   ratingsRenderer->updateBuffers();
   
   currentLevel = allSongs[currentSong];
}

#define TUTORIAL_FILENAME "RGB_Tutorial"
void nextState() {
   if (currentLevel->songTitle == TUTORIAL_FILENAME) {
      setState(new TutorialState(shipIndex));
   }
   else {
      setState(new InGameState(currentLevel->songTitle, currentLevel->bpm, shipIndex));
   }
}

void scrollUp() {
   
   int newShip = (currentSong - 1) % MAX_SONGS;
   if (newShip < 0) {
      newShip += MAX_SONGS;
   }
   currentSong = newShip;
}

void scrollDown() {
   //go down one index in the list
  
   currentSong = (currentSong + 1) % MAX_SONGS;
   
}

