#include "GuiResourceManager.h"

GuiResourceManager& GuiResourceManager::getInstance() {
  static GuiResourceManager fontManager;
  return fontManager;
}

GuiResourceManager::GuiResourceManager() {
  mName = "CLIENT: FONT_MANAGER";

  /////////////////////////
  // Fonts
  /////////////////////////
  loadFont(FONT::COURIER, "cour");
  loadFont(FONT::MORRIS, "MorrisRoman-Black");

  /////////////////////////
  // GUI textures
  /////////////////////////
  loadGuiTexture(GUI_TEXTURE::BUTTON, "ButtonM");
  loadGuiTexture(GUI_TEXTURE::BUTTON_LEFT, "ButtonL");
  loadGuiTexture(GUI_TEXTURE::BUTTON_RIGHT, "ButtonR");

  /////////////////////////
  // Sprite textures
  /////////////////////////
  // Characters
  loadSpriteTexture(Textures::CharacterBandana, "CharacterBandana1");
  loadSpriteTexture(Textures::CharacterChainHat, "CharacterChainHat");
  loadSpriteTexture(Textures::CharacterChainHood, "CharacterChainHood");
  loadSpriteTexture(Textures::CharacterClothHood, "CharacterClothHood");
  loadSpriteTexture(Textures::CharacterGoldenHelmet, "CharacterGoldenHelmet");
  loadSpriteTexture(Textures::CharacterLeatherCap, "CharacterLeatherCap");
  loadSpriteTexture(Textures::CharacterMetalHelmet, "CharacterMetalHelmet");
  loadSpriteTexture(Textures::CharacterMage, "mage");
  loadSpriteTexture(Textures::CharacterKnight, "knight3");
  loadSpriteTexture(Textures::CharacterSpearman, "spearman");

  // Walls
  loadSpriteTexture(Textures::HorizontalWall1, "wall1");
  loadSpriteTexture(Textures::VerticalWall1, "verticalWall1");

  // Ammo
  loadSpriteTexture(Textures::Bullet, "lightningBall");
  loadSpriteTexture(Textures::SwordSlash, "SwordSlash");
  loadSpriteTexture(Textures::LightningStrike, "lightningBolt");

  // Floors
  loadSpriteTexture(Textures::FloorWhole, "fantasy-tileset");
  loadSpriteTexture(Textures::FloorCracked, "fantasy-tileset");
  loadSpriteTexture(Textures::FloorSpikes, "fantasy-tileset");

  // Other
  loadSpriteTexture(Textures::Tombstone, "Tombstone");
  loadSpriteTexture(Textures::HealthPotion, "Potions/pt1Small");
  loadSpriteTexture(Textures::ManaPotion, "Potions/pt2Small");
  loadSpriteTexture(Textures::Ammo, "Potions/pt4Small");
}

sf::Font& GuiResourceManager::getFont(FONT font) {
  return mFontMap[font];
}

sf::Sprite GuiResourceManager::createSprite(GUI_TEXTURE texture) {
  sf::Sprite sprite(mGuiTextureMap[texture]);
  return sprite;
}

sf::Sprite GuiResourceManager::createSprite(Textures texture) {
  sf::Sprite sprite(mSpriteTextureMap[texture]);
  return sprite;
}

void GuiResourceManager::clear() {
  mFontMap.clear();
  mGuiTextureMap.clear();
  mSpriteTextureMap.clear();
}

void GuiResourceManager::loadFont(FONT fontType, std::string fileName) {
  sf::Font font;
  if (!font.loadFromFile("Client/Resources/Fonts/" + fileName + ".ttf")) {
    TRACE_ERROR("Could not load font");
  }
  mFontMap[fontType] = font;
}

void GuiResourceManager::loadGuiTexture(GUI_TEXTURE guiTexture, std::string fileName) {
  sf::Texture texture;
  if (!texture.loadFromFile("Client/Resources/Gui/" + fileName + ".png")) {
    TRACE_ERROR("Could not load font");
  }
  mGuiTextureMap[guiTexture] = texture;
}

void GuiResourceManager::loadSpriteTexture(Textures spriteTexture, std::string fileName) {
  sf::Texture texture;
  if (!texture.loadFromFile("Client/Resources/Sprites/" + fileName + ".png")) {
    TRACE_ERROR("Could not load font");
  }
  mSpriteTextureMap[spriteTexture] = texture;
}