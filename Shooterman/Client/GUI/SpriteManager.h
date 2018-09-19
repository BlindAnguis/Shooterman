#pragma once
#include <map>

#include <SFML/Graphics.hpp>

#include "../../Common/Trace.h"

#define SPRITE_FILE_PATH "Client/Resources/Sprites/"

class SpriteManager : Trace {
public:
  SpriteManager();
  ~SpriteManager();

  void loadSprites();
  std::shared_ptr<sf::Texture> loadTexture(std::string fileName);
  sf::Sprite get(int id) { 
    auto sprite = mSpriteMap.find(id);
    if (sprite == mSpriteMap.end()) {
      TRACE_ERROR("Sprite id " << id << " not found");
    }
    return sprite->second.first; }

private:
  std::map<int, std::pair<sf::Sprite, std::shared_ptr<sf::Texture>>> mSpriteMap;
};