#pragma once
#include <map>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../../Common/Trace.h"
#include "../../Common/Textures.h"

#define SPRITE_FILE_PATH "Client/Resources/Sprites/"

class SpriteManager : Trace {
public:
  SpriteManager();
  ~SpriteManager();

  void loadSprites();
  std::shared_ptr<sf::Texture> loadTexture(std::string fileName);
  sf::Sprite get(Textures texture) {
    auto sprite = mSpriteMap.find(texture);
    if (sprite == mSpriteMap.end()) {
      TRACE_ERROR("Sprite with texture " << static_cast<int>(texture) << " not found");
    }
    return sprite->second.first; 
  }

private:
  std::map<Textures, std::pair<sf::Sprite, std::shared_ptr<sf::Texture>>> mSpriteMap;
};