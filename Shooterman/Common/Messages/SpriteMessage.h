#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

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
  SpriteMessage() {}
  ~SpriteMessage() {}

  void addSpriteData(SpriteData data) {
    mSpriteData.push_back(data);
  }

  SpriteData getSpriteData(unsigned int position) {
    if (position >= mSpriteData.size() || position < 0) {
      SpriteData data;
      data.textureId = Textures::Unknown;
      return data;
    }
    return mSpriteData[position];
  }
  
  sf::Packet pack() {
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

  void unpack(sf::Packet packet) {
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

  int getSize() {
    return mSize;
  }

private:
  int mSize;
  std::string mName = "TRACE";
  std::vector<SpriteData> mSpriteData;
};