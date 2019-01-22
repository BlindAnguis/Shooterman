#include "SpriteManager.h"

SpriteManager::SpriteManager() { mName = "CLIENT: SPRITE_MANAGER"; }

SpriteManager::~SpriteManager() { }

void SpriteManager::loadSprites() {

  mSpriteMap[Textures::CharacterBandana] = createSprite("CharacterBandana.png");
  mSpriteMap[Textures::CharacterChainHat] = createSprite("CharacterChainHat.png");
  mSpriteMap[Textures::CharacterChainHood] = createSprite("CharacterChainHood.png");
  mSpriteMap[Textures::CharacterClothHood] = createSprite("CharacterClothHood.png");
  mSpriteMap[Textures::CharacterGoldenHelmet] = createSprite("CharacterGoldenHelmet.png");
  mSpriteMap[Textures::CharacterLeatherCap] = createSprite("CharacterLeatherCap.png");
  mSpriteMap[Textures::CharacterMetalHelmet] = createSprite("CharacterMetalHelmet.png");
  
  mSpriteMap[Textures::HorizontalWall1] = createSprite("wall1.png");
  mSpriteMap[Textures::VerticalWall1] = createSprite("verticalWall1.png");

  mSpriteMap[Textures::Bullet] = createSprite("Bullet.png");

  mSpriteMap[Textures::Tombstone] = createSprite("Tombstone.png");
}

std::pair<sf::Sprite, std::shared_ptr<sf::Texture>> SpriteManager::createSprite(std::string filename) {
  std::shared_ptr<sf::Texture> texture = loadTexture(filename);
  sf::Sprite sprite;
  sprite.setTexture(*texture);
  return std::make_pair(sprite, texture);
}

std::shared_ptr<sf::Texture> SpriteManager::loadTexture(std::string fileName) {
  std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
  if (!texture->loadFromFile(SPRITE_FILE_PATH + fileName)) {
    TRACE_ERROR("could not load file " << SPRITE_FILE_PATH << fileName);
  }
  return texture;
}