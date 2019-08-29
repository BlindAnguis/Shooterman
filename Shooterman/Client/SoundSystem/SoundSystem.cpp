#include "SoundSystem.h"

#include "../../Common/Interfaces.h"
#include "../../Common/Messages/SoundMessage.h"

SoundSystem::SoundSystem(std::shared_ptr<MessageHandler> messageHandler) : mCurrentGameState(GAME_STATE::NO_STATE), mMessageHandler(messageHandler) {
  mName = "CLIENT: SOUND_SYSTEM";
}

SoundSystem::~SoundSystem() { }

void SoundSystem::start() {
  mSoundSubcription.addSignalCallback(MessageId::SOUND_LIST, std::bind(&SoundSystem::handleSoundListMessage, this, std::placeholders::_1));
  mSoundSubcription.addSignalCallback(MessageId::CHANGE_GAME_STATE, std::bind(&SoundSystem::handleChangeGameStateMessage, this, std::placeholders::_1));

  GameStateMessage gsm(GAME_STATE::MAIN_MENU);
  mSoundSubcription.sendMessage(gsm.pack());

  mSubscribedToSounds = mMessageHandler->subscribeTo(Interfaces::CLIENT_GAME_STATE, &mSoundSubcription);

  startListenToSubscriber(&mSoundSubcription);

  loadSounds();
}

void SoundSystem::run() {
  if (!mSubscribedToSounds) {
    mSubscribedToSounds = mMessageHandler->subscribeTo(Interfaces::CLIENT_SOUND_LIST, &mSoundSubcription);
  }

  while (!mPlayQueue.empty() && mPlayQueue.front().getStatus() == sf::Sound::Stopped) {
    //TRACE_INFO("Sound has stopped");
    mPlayQueue.pop();
  }
  sf::sleep(sf::milliseconds(1));
}

void SoundSystem::stop() {
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mSoundSubcription);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_SOUND_LIST, &mSoundSubcription);

  stopListenToSubscriber(&mSoundSubcription);
}

void SoundSystem::loadSounds() {
  loadSoundBuffer(Sounds::SlashLong1, "Client/Resources/Sounds/SoundEffects/swish_2.wav");
  loadSoundBuffer(Sounds::SlashLong2, "Client/Resources/Sounds/SoundEffects/swish_4.wav");
  loadSoundBuffer(Sounds::Hit1, "Client/Resources/Sounds/SoundEffects/fall.wav");
  loadSoundBuffer(Sounds::Death, "Client/Resources/Sounds/SoundEffects/hit_2.wav");

  loadMusic(GAME_STATE::MAIN_MENU, "Client/Resources/Sounds/Music/mainMenu.wav");
  loadMusic(GAME_STATE::LOBBY, "Client/Resources/Sounds/Music/Lobby_Heroic_Demise_New.ogg");
  loadMusic(GAME_STATE::CLIENT_LOBBY, "Client/Resources/Sounds/Music/Lobby_Heroic_Demise_New.ogg");
  loadMusic(GAME_STATE::PLAYING, "Client/Resources/Sounds/Music/playing.ogg");
}

void SoundSystem::loadSoundBuffer(Sounds sound, std::string filename) {
  sf::SoundBuffer soundBuffer;
  if (!soundBuffer.loadFromFile(filename)) {
    TRACE_ERROR("Couldn't load sound buffer: " << filename);
  }
  mSoundBuffers.emplace(sound, soundBuffer);
}

void SoundSystem::loadMusic(GAME_STATE gameState, std::string filename) {
  auto music = std::make_shared<sf::Music>();
  if (!music->openFromFile(filename)) {
    TRACE_ERROR("Couldn't load music: " << filename);
  }
  music->setLoop(true);
  mBackgroundSoundBuffers.emplace(gameState, music);
}

void SoundSystem::handleChangeGameStateMessage(sf::Packet& message) {
  GameStateMessage gsm(message);

  if (gsm.getGameState() == mCurrentGameState) {
    return;
  }

  switch (gsm.getGameState()) {
  case GAME_STATE::MAIN_MENU:
    if (mSubscribedToSounds) {
      mMessageHandler->unsubscribeTo(Interfaces::CLIENT_SOUND_LIST, &mSoundSubcription);
      mSubscribedToSounds = false;
    }
  case GAME_STATE::LOBBY:
  case GAME_STATE::CLIENT_LOBBY:
  case GAME_STATE::PLAYING:
    if (mCurrentGameState != GAME_STATE::NO_STATE) {
      mBackgroundSoundBuffers.at(mCurrentGameState)->stop();
    }
    mBackgroundSoundBuffers.at(gsm.getGameState())->play();
    mCurrentGameState = gsm.getGameState();
    break;
  default:
    // Dont change music
    break;
  }
}

void SoundSystem::handleSoundListMessage(sf::Packet& message) {
  SoundMessage sm;
  sm.unpack(message);
  for (auto i = 0; i < sm.getSize(); i++) {
    //TRACE_INFO("Creating sound: " << static_cast<int>(sm.getSound(i)));
    mPlayQueue.push(sf::Sound());
    mPlayQueue.back().setBuffer(mSoundBuffers.at(sm.getSound(i)));
    mPlayQueue.back().play();
  }
}