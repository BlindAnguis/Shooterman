#include "SpriteManager.h"

SpriteManager::SpriteManager() { 
  mName = "CLIENT: SPRITE_MANAGER";

  // Characters
  mTexturePathMap[Textures::CharacterBandana] = "CharacterBandana1.png";
  mTexturePathMap[Textures::CharacterChainHat] = "CharacterChainHat.png";
  mTexturePathMap[Textures::CharacterChainHood] = "CharacterChainHood.png";
  mTexturePathMap[Textures::CharacterClothHood] = "CharacterClothHood.png";
  mTexturePathMap[Textures::CharacterGoldenHelmet] = "CharacterGoldenHelmet.png";
  mTexturePathMap[Textures::CharacterLeatherCap] = "CharacterLeatherCap.png";
  mTexturePathMap[Textures::CharacterMetalHelmet] = "CharacterMetalHelmet.png";
  mTexturePathMap[Textures::CharacterMage] = "mage.png";
  mTexturePathMap[Textures::CharacterKnight] = "knight3.png";
  mTexturePathMap[Textures::CharacterSpearman] = "spearman.png";

  // Walls
  mTexturePathMap[Textures::HorizontalWall1] = "wall1.png";
  mTexturePathMap[Textures::VerticalWall1] = "verticalWall1.png";

  // Ammo
  //mTexturePathMap[Textures::Bullet] = "waterSpell.png";
  mTexturePathMap[Textures::Bullet] = "lightningBall.png";
  mTexturePathMap[Textures::SwordSlash] = "SwordSlash.png";
  mTexturePathMap[Textures::LightningStrike] = "lightningBolt.png";

  // Floors
  mTexturePathMap[Textures::FloorWhole] = "fantasy-tileset.png";
  mTexturePathMap[Textures::FloorCracked] = "fantasy-tileset.png";
  mTexturePathMap[Textures::FloorSpikes] = "fantasy-tileset.png";

  // Other
  mTexturePathMap[Textures::Tombstone] = "Tombstone.png";
  mTexturePathMap[Textures::HealthPotion] = "Potions/pt1Small.png";
  mTexturePathMap[Textures::ManaPotion] = "Potions/pt2Small.png";
  mTexturePathMap[Textures::Ammo] = "Potions/pt4Small.png";

  for (auto texture : mTexturePathMap) {
    loadTexture(texture.first, texture.second);
    createSprite(texture.first, texture.second);
  }
}

SpriteManager::~SpriteManager() { }

void SpriteManager::createSprite(Textures textureId, std::string filename) {
  sf::Sprite sprite;
  sprite.setTexture(*mTextureMap[textureId]);
  mSpriteMap[textureId] = sprite;
}

void SpriteManager::loadTexture(Textures textureId, std::string fileName) {
  mTextureMap[textureId] = new sf::Texture();
  if (!mTextureMap[textureId]->loadFromFile(SPRITE_FILE_PATH + fileName)) {
    TRACE_ERROR("could not load file " << SPRITE_FILE_PATH << "CharacterBandana1.png");
  }
}

sf::Sprite& SpriteManager::getSprite(Textures textureId) {
  return mSpriteMap[textureId];
}