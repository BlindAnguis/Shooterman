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

  sf::Sprite& getSprite(Textures textureId);

  SpriteManager(SpriteManager const&) = delete;
  void operator=(SpriteManager const&) = delete;

private:
  std::map<Textures, sf::Sprite> mSpriteMap;
  std::map<Textures, sf::Texture*> mTextureMap;
  std::map<Textures, std::string> mTexturePathMap;

  void createSprite(Textures textureId, std::string filename);
  void loadTexture(Textures textureId, std::string fileName);

};