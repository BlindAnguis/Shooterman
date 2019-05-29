#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
  mName = "CLIENT: SOUND_SYSTEM";
  loadSounds();
}

SoundSystem::~SoundSystem()
{
  MessageHandler::get().unpublishInterface("ClientSoundList");
}

void SoundSystem::loadSounds() {
  sf::SoundBuffer slashLong1Buffer;
  if (!slashLong1Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/swish_2.wav")) {
    TRACE_ERROR("Couldn't load Client/Resources/Sounds/SoundEffects/swish_2.wav");
  }
  mSoundBuffers.emplace(Sounds::SlashLong1, slashLong1Buffer);

  sf::SoundBuffer slashLong2Buffer;
  if (!slashLong2Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/swish_4.wav")) {
    TRACE_ERROR("Couldn't load Client/Resources/Sounds/SoundEffects/swish_4.wav");
  }
  mSoundBuffers.emplace(Sounds::SlashLong2, slashLong2Buffer);

  sf::SoundBuffer hit1Buffer;
  if (!hit1Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/fall.wav")) {
    TRACE_ERROR("Couldn't load Client/Resources/Sounds/SoundEffects/fall.wav");
  }
  mSoundBuffers.emplace(Sounds::Hit1, hit1Buffer);
}

void SoundSystem::update() {
  if (!mSubscribedToSounds) {
    mSubscribedToSounds = MessageHandler::get().subscribeTo("ClientSoundList", &mSoundSubcription);
  }

  auto soundsToPlay = mSoundSubcription.getMessageQueue();
  while (!soundsToPlay.empty()) {
    int id = -1;
    auto soundPacket = soundsToPlay.front();
    soundsToPlay.pop();
    soundPacket >> id;
    if (id == SOUND_LIST) {
      SoundMessage sm;
      sm.unpack(soundPacket);
      for (auto i = 0; i < sm.getSize(); i++) {
        mPlayingSounds.emplace_back(sf::Sound());
        mPlayingSounds.back().setBuffer(mSoundBuffers.at(sm.getSound(i)));
        mPlayingSounds.back().play();
      }
    } else {
      TRACE_ERROR("Got the wrong message, got message with id: " << id << " but should have got id: " << SOUND_LIST);
    }    
  }

  auto it = mPlayingSounds.begin();
  
  while (it != mPlayingSounds.end()) {
    if (it->getStatus() == sf::Sound::Stopped) {
      it = mPlayingSounds.erase(it);
    } else {
      ++it;
    }
  }
}
