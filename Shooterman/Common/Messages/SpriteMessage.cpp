#include "SpriteMessage.h"

SpriteMessage::SpriteMessage() {}

SpriteMessage::SpriteMessage(sf::Packet packet) {
  unpack(packet);
}

SpriteMessage::~SpriteMessage() {}

void SpriteMessage::addSpriteData(SpriteData data) {
  mSpriteData.push_back(data);
}

SpriteData SpriteMessage::getSpriteData(unsigned int position) {
  if (position >= mSpriteData.size() || position < 0) {
    SpriteData data;
    data.textureId = Textures::Unknown;
    return data;
  }
  return mSpriteData[position];
}

sf::Packet SpriteMessage::pack() {
  sf::Packet packet;
  packet << SPRITE_LIST;
  packet << mSpriteData.size();

  for (SpriteData data : mSpriteData) {
    //std::cout << "TMP: " << SPRITE_LIST << " Size: " << mSpriteData.size() << " ID: " << mSpriteData.front().first << std::endl;
    packet << static_cast<int>(data.textureId);
    packet << data.position.x << data.position.y;
    packet << data.texturePosition.left << data.texturePosition.top << data.texturePosition.width << data.texturePosition.height;
    packet << data.rotation;
  }
  return packet;
}

void SpriteMessage::unpack(sf::Packet packet) {
  mSpriteData.clear();
  packet >> mSize;
  for (int i = 0; i < mSize; i++) {
    int textureId;
    SpriteData data;
    packet >> textureId;
    data.textureId = static_cast<Textures>(textureId);
    packet >> data.position.x >> data.position.y;
    packet >> data.texturePosition.left >> data.texturePosition.top >> data.texturePosition.width >> data.texturePosition.height;
    packet >> data.rotation;
    mSpriteData.push_back(data);
  }
}

int SpriteMessage::getSize() {
  return mSize;
}