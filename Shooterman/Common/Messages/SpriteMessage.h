#pragma once

#include <queue>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../MessageId.h"

class SpriteMessage {
public:
  SpriteMessage() {}
  ~SpriteMessage() {}

  void addSpriteData(int ID, sf::Vector2f position) {
    mSpriteData.push(std::make_pair(ID, position));
  }

  std::pair<int, sf::Vector2f> getSpriteData() {
    if (mSpriteData.empty()) {
      return std::make_pair(-1, sf::Vector2f());
    }
    std::pair<int, sf::Vector2f> spriteData = mSpriteData.front();
    mSpriteData.pop();
    return spriteData;
  }
  
  sf::Packet pack() {
    sf::Packet packet;
    packet << SPRITE_LIST;
    packet << mSpriteData.size();
    while (!mSpriteData.empty()) {
      //std::cout << "TMP: " << SPRITE_LIST << " Size: " << mSpriteData.size() << " ID: " << mSpriteData.front().first << std::endl;
      packet << mSpriteData.front().first << mSpriteData.front().second.x << mSpriteData.front().second.y;
      mSpriteData.pop();
    }
    return packet;
  }

  void unpack(sf::Packet packet) {
    int size;
    packet >> size;
    for (int i = 0; i < size; i++) {
      int ID;
      sf::Vector2f position;
      packet >> ID >> position.x >> position.y;
      mSpriteData.push(std::make_pair(ID, position));
    }
  }

private:
  std::string mName = "TRACE";
  std::queue<std::pair<int, sf::Vector2f>> mSpriteData;
};