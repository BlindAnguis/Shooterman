#include "SoundSystem.h"
#include "../../Common/Interfaces.h"

SoundSystem::SoundSystem()
{
  mName = "CLIENT: SOUND_SYSTEM";
  loadSounds();
  mPathsToBackgroundMusic.emplace(Sounds::MainMenuBackgroundSong, "Client/Resources/Sounds/Music/mainMenu.wav");
  mPathsToBackgroundMusic.emplace(Sounds::LobbyBackgroundSong, "Client/Resources/Sounds/Music/Lobby_Heroic_Demise_New.ogg");
  mPathsToBackgroundMusic.emplace(Sounds::PlayingBackgroundSong, "Client/Resources/Sounds/Music/playing.ogg");
  mCurrentBackgroundMusic = Sounds::Unkown;
}

SoundSystem::~SoundSystem()
{
  MessageHandler::get().unpublishInterface("ClientSoundList");
}

void SoundSystem::unsubscribeToSoundList() {
  if (mSubscribedToSounds) {
    MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_SOUND_LIST, &mSoundSubcription);
    mSubscribedToSounds = false;
  }
}

void SoundSystem::loadSounds() {
  sf::SoundBuffer slashLong1Buffer;
  if (!slashLong1Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/swish_2.wav")) {
    TRACE_ERROR("Couldn't load slashLong1Buffer: Client/Resources/Sounds/SoundEffects/swish_2.wav");
  }
  mSoundBuffers.emplace(Sounds::SlashLong1, slashLong1Buffer);

  sf::SoundBuffer slashLong2Buffer;
  if (!slashLong2Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/swish_4.wav")) {
    TRACE_ERROR("Couldn't load slashLong2Buffer: Client/Resources/Sounds/SoundEffects/swish_4.wav");
  }
  mSoundBuffers.emplace(Sounds::SlashLong2, slashLong2Buffer);

  sf::SoundBuffer hit1Buffer;
  if (!hit1Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/fall.wav")) {
    TRACE_ERROR("Couldn't load hit1Buffer: Client/Resources/Sounds/SoundEffects/fall.wav");
  }
  mSoundBuffers.emplace(Sounds::Hit1, hit1Buffer);

  sf::SoundBuffer deathBuffer;
  if (!deathBuffer.loadFromFile("Client/Resources/Sounds/SoundEffects/hit_2.wav")) {
    TRACE_ERROR("Couldn't load deathBuffer: Client/Resources/Sounds/SoundEffects/hit_2.wav");
  }
  mSoundBuffers.emplace(Sounds::Death, deathBuffer);
}

void SoundSystem::update() {
  if (!mSubscribedToSounds) {
    mSubscribedToSounds = MessageHandler::get().subscribeTo(Interfaces::CLIENT_SOUND_LIST, &mSoundSubcription);
  }

  auto soundsToPlay = mSoundSubcription.getMessageQueue();
  while (!soundsToPlay.empty()) {
    int id = -1;
    auto soundPacket = soundsToPlay.front();
    soundsToPlay.pop();
    soundPacket >> id;
    if (id == MessageId::SOUND_LIST) {
      SoundMessage sm;
      sm.unpack(soundPacket);
      for (auto i = 0; i < sm.getSize(); i++) {
        //TRACE_INFO("Creating sound: " << static_cast<int>(sm.getSound(i)));
        mPlayQueue.push(sf::Sound());
        mPlayQueue.back().setBuffer(mSoundBuffers.at(sm.getSound(i)));
        mPlayQueue.back().play();
      }
    } else {
      TRACE_ERROR("Got the wrong message, got message with id: " << id << " but should have got id: " << MessageId::SOUND_LIST);
    }    
  }

  while (!mPlayQueue.empty() && mPlayQueue.front().getStatus() == sf::Sound::Stopped) {
    //TRACE_INFO("Sound has stopped");
    mPlayQueue.pop();
  }
}

void SoundSystem::setBackgroundMusic(Sounds backgroundMusic) {
  auto path = mPathsToBackgroundMusic.at(backgroundMusic);
  if (!mBackgroundMusic.openFromFile(path)) {
    TRACE_ERROR("Couldn't load Sound: " << (int)backgroundMusic << "with path: " << path);
  } else {
    mCurrentBackgroundMusic = backgroundMusic;
  }
}

void SoundSystem::stopBackgroundMusic() {
  mBackgroundMusic.stop();
}

void SoundSystem::pauseBackgroundMusic() {
  mBackgroundMusic.pause();
}

void SoundSystem::startBackGroundMusic() {
  mBackgroundMusic.setLoop(true);
  mBackgroundMusic.play();
}

bool SoundSystem::isBackgroundMusicPlaying(Sounds backgroundMusic) {
  if (mCurrentBackgroundMusic == backgroundMusic) {
    return mBackgroundMusic.getStatus() == sf::Sound::Playing;
  }
  return false;
}
