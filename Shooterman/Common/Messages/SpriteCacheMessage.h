#pragma once

#include <vector>

#include "SpriteMessage.h"

class SpriteCacheMessage : public SpriteMessage {
public:
  SpriteCacheMessage();
  ~SpriteCacheMessage();

  void addSpriteData(SpriteData data);
  SpriteData getSpriteData(unsigned int position);

  sf::Packet pack();
  void unpack(sf::Packet packet);

  int getSize();

private:
  int mSize;
  std::vector<SpriteData> mSpriteData;
};