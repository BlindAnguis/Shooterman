#pragma once

#include "../../Common/Sounds.h"
#include "../../Common/Trace.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/Process/Runnable.h"

#include "SFML/Audio.hpp"

#include <vector>
#include <map>

class SoundSystem : public Runnable, Trace {
public:
  SoundSystem(std::shared_ptr<MessageHandler> messageHandler);
  ~SoundSystem();
  
  void start() override;
  void run() override;
  void stop() override;

private:
  bool mSubscribedToSounds = false;
  GAME_STATE mCurrentGameState;
  std::vector<sf::Sound> mPlayingSounds;
  std::queue<sf::Sound> mPlayQueue;
  std::map<Sounds, sf::SoundBuffer> mSoundBuffers;
  std::map<GAME_STATE, std::shared_ptr<sf::Music>> mBackgroundSoundBuffers;
  Subscriber mSoundSubcription;
  std::shared_ptr<MessageHandler> mMessageHandler;

  void loadSounds();
  void loadSoundBuffer(Sounds sound, std::string filename);
  void loadMusic(GAME_STATE gameState, std::string filename);
  void handleChangeGameStateMessage(sf::Packet& message);
  void handleSoundListMessage(sf::Packet& message);
};

