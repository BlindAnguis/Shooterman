#include "FontManager.h"

FontManager& FontManager::getInstance() {
  static FontManager fontManager;
  return fontManager;
}

FontManager::FontManager() {
  mName = "CLIENT: FONT_MANAGER";
  sf::Font font1;
  if (!font1.loadFromFile("Client/Resources/Fonts/cour.ttf")) {
    TRACE_ERROR("Could not load font");
  }
  mFontMap[FONT::COURIER] = font1;

  sf::Font font2;
  if (!font2.loadFromFile("Client/Resources/Fonts/MorrisRoman-Black.ttf")) {
    TRACE_ERROR("Could not load font");
  }
  mFontMap[FONT::MORRIS] = font2;


  sf::Texture textureM;
  if (!textureM.loadFromFile("Client/Resources/Gui/ButtonM.png")) {
    TRACE_ERROR("Could not load texture");
  }
  mTextureMap[TEXTURE::BUTTON] = textureM;

  sf::Texture textureL;
  if (!textureL.loadFromFile("Client/Resources/Gui/ButtonL.png")) {
    TRACE_ERROR("Could not load texture");
  }
  mTextureMap[TEXTURE::BUTTON_LEFT] = textureL;

  sf::Texture textureR;
  if (!textureR.loadFromFile("Client/Resources/Gui/ButtonR.png")) {
    TRACE_ERROR("Could not load texture");
  }
  mTextureMap[TEXTURE::BUTTON_RIGHT] = textureR;
}

sf::Font& FontManager::getFont(FONT font) {
  return mFontMap[font];
}

sf::Sprite FontManager::createSprite(TEXTURE texture) {
  sf::Sprite sprite(mTextureMap[texture]);
  return sprite;
}

void FontManager::clear() {
  mFontMap.clear();
  mTextureMap.clear();
}