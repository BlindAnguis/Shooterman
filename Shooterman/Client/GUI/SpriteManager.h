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
  sf::Texture* loadTexture(std::string fileName);
  sf::Sprite get(int id) { return mSpriteMap.at(id).first; }

private:
  std::map<int, std::pair<sf::Sprite, sf::Texture*>> mSpriteMap;
};