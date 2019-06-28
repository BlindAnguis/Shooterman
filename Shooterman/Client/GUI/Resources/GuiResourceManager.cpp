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
  loadGuiTexture(GUI_TEXTURE::BUTTON_PRESSED, "ButtonMPressed");
  loadGuiTexture(GUI_TEXTURE::BUTTON_LEFT_PRESSED, "ButtonLPressed");
  loadGuiTexture(GUI_TEXTURE::BUTTON_RIGHT_PRESSED, "ButtonRPressed");

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
  mSpriteTextureRectMap[Textures::CharacterMage] = sf::IntRect(0, 64 * 10 + 14, 64, 50);
  loadSpriteTexture(Textures::CharacterKnight, "knight3");
  mSpriteTextureRectMap[Textures::CharacterKnight] = sf::IntRect(0, 64 * 10 + 14, 64, 50);
  loadSpriteTexture(Textures::CharacterSpearman, "spearman");
  mSpriteTextureRectMap[Textures::CharacterSpearman] = sf::IntRect(0, 64 * 10 + 14, 64, 50);
  loadSpriteTexture(Textures::CharacterArcher, "archer");
  mSpriteTextureRectMap[Textures::CharacterArcher] = sf::IntRect(0, 64 * 10 + 14, 64, 50);
  // Walls
  loadSpriteTexture(Textures::HorizontalWall1, "wall1");
  mSpriteTextureRectMap[Textures::HorizontalWall1] = sf::IntRect(0, 0, 32, 32);
  loadSpriteTexture(Textures::VerticalWall1, "verticalWall1");

  // Ammo
  loadSpriteTexture(Textures::Bullet, "lightningBall");
  loadSpriteTexture(Textures::Arrow, "arrow");
  loadSpriteTexture(Textures::SwordSlash, "SwordSlash");
  loadSpriteTexture(Textures::LightningStrike, "lightningBolt");

  // Floors
  loadSpriteTexture(Textures::FloorWhole, "fantasy-tileset");
  mSpriteTextureRectMap[Textures::FloorWhole] = sf::IntRect(32 * 2, 32 * 1, 32, 32);
  loadSpriteTexture(Textures::FloorCracked, "fantasy-tileset");
  mSpriteTextureRectMap[Textures::FloorCracked] = sf::IntRect(32 * 3, 32 * 1, 32, 32);
  loadSpriteTexture(Textures::FloorSpikes, "fantasy-tileset");
  mSpriteTextureRectMap[Textures::FloorSpikes] = sf::IntRect(32 * 3, 32 * 3, 32, 32);

  // Other
  loadSpriteTexture(Textures::Tombstone, "Tombstone");
  loadSpriteTexture(Textures::HealthPotion, "Potions/pt1Small");
  loadSpriteTexture(Textures::ManaPotion, "Potions/pt2Small");
  loadSpriteTexture(Textures::Ammo, "Potions/pt4Small");
}

GuiResourceManager::~GuiResourceManager() { }

sf::Font& GuiResourceManager::getFont(FONT font) {
  return mFontMap[font];
}

sf::Sprite GuiResourceManager::createSprite(GUI_TEXTURE texture) {
  sf::Sprite sprite(mGuiTextureMap[texture]);
  return sprite;
}

sf::Sprite GuiResourceManager::createSprite(Textures texture) {
  sf::Sprite sprite(mSpriteTextureMap[texture]);
  if (mSpriteTextureRectMap.find(texture) != mSpriteTextureRectMap.end()) {
    sprite.setTextureRect(mSpriteTextureRectMap[texture]);
  }
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