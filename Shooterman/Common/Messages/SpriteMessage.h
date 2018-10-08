#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Message.h"
#include "../MessageId.h"

class SpriteMessage : Message {
public:
  SpriteMessage() {}
  ~SpriteMessage() {}

  void addSpriteData(int ID, sf::Vector2f position) {
    mSpriteData.push_back(std::make_pair(ID, position));
  }

  std::pair<int, sf::Vector2f> getSpriteData(int position) {
    if (position >= mSpriteData.size() || position < 0) {
      return std::make_pair(-1, sf::Vector2f());
    }
    return mSpriteData[position];
  }
  
  sf::Packet pack() {
    sf::Packet packet;
    packet << SPRITE_LIST;
    packet << mSpriteData.size();
    for (std::pair<int, sf::Vector2f> sprite : mSpriteData) {
      //std::cout << "TMP: " << SPRITE_LIST << " Size: " << mSpriteData.size() << " ID: " << mSpriteData.front().first << std::endl;
      packet << sprite.first << sprite.second.x << sprite.second.y;;
    }
    return packet;
  }

  void unpack(sf::Packet packet) {
    packet >> mSize;
    for (int i = 0; i < mSize; i++) {
      int ID;
      sf::Vector2f position;
      packet >> ID >> position.x >> position.y;
      mSpriteData.push_back(std::make_pair(ID, position));
    }
  }

  int getSize() {
    return mSize;
  }

private:
  int mSize;
  std::string mName = "TRACE";
  std::vector<std::pair<int, sf::Vector2f>> mSpriteData;
};