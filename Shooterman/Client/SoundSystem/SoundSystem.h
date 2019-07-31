#pragma once

#include "../../Common/Sounds.h"
#include "../../Common/Trace.h"
#include "../Network/NetworkHandler.h"
#include "../../Common/Messages/SoundMessage.h"

#include "SFML/Audio.hpp"

#include <vector>
#include <map>

class SoundSystem : Trace {
public:
  SoundSystem();
  ~SoundSystem();
  void update();
  void setBackgroundMusic(Sounds backgroundMusic);
  void stopBackgroundMusic();
  void pauseBackgroundMusic();
  void startBackGroundMusic();
  bool isBackgroundMusicPlaying(Sounds backgroundMusic);
  void unsubscribeToSoundList();
private:
  std::vector<sf::Sound> mPlayingSounds;
  std::queue<sf::Sound> mPlayQueue;
  std::map<Sounds, sf::SoundBuffer> mSoundBuffers;
  bool mSubscribedToSounds = false;
  Subscriber mSoundSubcription;
  sf::Music mBackgroundMusic;
  Sounds mCurrentBackgroundMusic;
  std::map<Sounds, std::string> mPathsToBackgroundMusic;

  void loadSounds();
  void handleSoundListMessage(sf::Packet message);
};

