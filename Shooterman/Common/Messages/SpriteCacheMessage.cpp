#include "SpriteCacheMessage.h"

SpriteCacheMessage::SpriteCacheMessage() {}

SpriteCacheMessage::SpriteCacheMessage(sf::Packet packet) {
  unpack(packet);
}

SpriteCacheMessage::~SpriteCacheMessage() {}

void SpriteCacheMessage::addSpriteData(SpriteData data) {
  mSpriteData.push_back(data);
}

SpriteData SpriteCacheMessage::getSpriteData(unsigned int position) {
  if (position >= mSpriteData.size() || position < 0) {
    SpriteData data;
    data.textureId = Textures::Unknown;
    return data;
  }
  return mSpriteData[position];
}

sf::Packet SpriteCacheMessage::pack() {
  sf::Packet packet;
  packet << MessageId::SPRITE_LIST_CACHE;
  packet << mSpriteData.size();

  for (SpriteData data : mSpriteData) {
    packet << static_cast<int>(data.textureId);
    packet << data.position.x << data.position.y;
    packet << data.texturePosition.left << data.texturePosition.top << data.texturePosition.width << data.texturePosition.height;
    packet << data.rotation;
  }
  return packet;
}

void SpriteCacheMessage::unpack(sf::Packet packet) {
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

int SpriteCacheMessage::getSize() {
  return mSize;
}