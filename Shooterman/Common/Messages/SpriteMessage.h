#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Message.h"
#include "../MessageId.h"
#include "../../Common/Textures.h"

typedef struct {
  Textures textureId;
  sf::Vector2f position;
  sf::IntRect texturePosition;
  float rotation;
} SpriteData;

class SpriteMessage : Message {
public:
  SpriteMessage();
  ~SpriteMessage();

  void addSpriteData(SpriteData data);

  SpriteData getSpriteData(unsigned int position);
  
  sf::Packet pack();

  void unpack(sf::Packet packet);

  int getSize();

private:
  int mSize;
  std::vector<SpriteData> mSpriteData;
};