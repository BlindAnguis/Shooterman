#pragma once

#include "../../Common/Sounds.h"
#include "../../Common/Trace.h"
#include "../Network/NetworkHandler.h"
#include "../../Common/Messages/SoundMessage.h"

#include "SFML/Audio.hpp"

#include <vector>
#include <map>

class SoundSystem : Trace
{
public:
  SoundSystem();
  ~SoundSystem();
  void update();
private:
  std::vector<sf::Sound> mPlayingSounds;
  std::map<Sounds, sf::SoundBuffer> mSoundBuffers;
  bool mSubscribedToSounds = false;
  void loadSounds();
  Subscriber mSoundSubcription;
};

