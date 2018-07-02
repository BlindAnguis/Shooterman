#include "SpriteManager.h"

SpriteManager::SpriteManager() { mName = "GUI"; }

SpriteManager::~SpriteManager() {
  for (auto it = mSpriteMap.begin(); it != mSpriteMap.end(); it++) {
    delete it->second.second;
  }
}

void SpriteManager::loadSprites() {
  sf::Texture* texture = loadTexture("Player.png");
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  mSpriteMap[33] = std::make_pair(sprite, texture);
}

sf::Texture* SpriteManager::loadTexture(std::string fileName) {
  sf::Texture* playerTexture = new sf::Texture();
  if (!playerTexture->loadFromFile(SPRITE_FILE_PATH + fileName)) {
    TRACE_ERROR("could not load file " << SPRITE_FILE_PATH << fileName);
  }
  return playerTexture;
}