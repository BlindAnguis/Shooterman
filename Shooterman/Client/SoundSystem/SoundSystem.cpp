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
    TRACE_INFO("Getting sound packet");
    int id = -1;
    auto soundPacket = soundsToPlay.front();
    soundsToPlay.pop();
    soundPacket >> id;
    if (id == SOUND_LIST) {
      SoundMessage sm;
      sm.unpack(soundPacket);
      for (auto i = 0; i < sm.getSize(); i++) {
        //TRACE_INFO("Creating sound: " << static_cast<int>(sm.getSound(i)));
        mPlayQueue.push(sf::Sound());
        mPlayQueue.back().setBuffer(mSoundBuffers.at(sm.getSound(i)));
        mPlayQueue.back().play();
      }
    } else {
      TRACE_ERROR("Got the wrong message, got message with id: " << id << " but should have got id: " << SOUND_LIST);
    }    
  }

  for (auto sound : mPlayingSounds) {
    //TRACE_INFO("Sound again has status: " << sound.getStatus() << " currant duration is: " << (int)sound.getPlayingOffset().asMilliseconds() << " and total duration is: " << (int)sound.getBuffer()->getDuration().asMilliseconds());
  }

  while (!mPlayQueue.empty() && mPlayQueue.front().getStatus() == sf::Sound::Stopped) {
    TRACE_INFO("Sound has stopped");
    mPlayQueue.pop();
  }
}

class Audio {
private:
  sf::SoundBuffer mBuffer;
  sf::Sound mSound;
public:
  void init(sf::SoundBuffer buffer) {        //Initialize audio
    mBuffer = buffer;
    mSound.setBuffer(mBuffer);
  }
  void play() {
    mSound.play();       // Play queued audio
  }
  void stop() {
    mSound.stop();
  }
  //void setVolume(), void setPitch() ....
};
