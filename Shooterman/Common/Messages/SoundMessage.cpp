#include "SoundMessage.h"

SoundMessage::SoundMessage() {}

SoundMessage::SoundMessage(sf::Packet packet) {
  unpack(packet);
}

SoundMessage::~SoundMessage() {}

void SoundMessage::addSound(Sounds sound) {
  mSounds.push_back(sound);
}

Sounds SoundMessage::getSound(unsigned int position) {
  if (position >= mSounds.size() || position < 0) {
    return Sounds::Unkown;
  }
  return mSounds[position];
}

sf::Packet SoundMessage::pack() {
  sf::Packet packet;
  packet << SOUND_LIST;
  packet << mSounds.size();

  for (auto sound : mSounds) {
    packet << static_cast<int>(sound);
  }
  return packet;
}

void SoundMessage::unpack(sf::Packet packet) {
  mSounds.clear();
  packet >> mSize;
  for (int i = 0; i < mSize; i++) {
    int soundAsInt;
    packet >> soundAsInt;
    mSounds.push_back(static_cast<Sounds>(soundAsInt));
  }
}

int SoundMessage::getSize() {
  return mSize;
}
