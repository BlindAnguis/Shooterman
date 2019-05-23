#include "FontManager.h"

FontManager & FontManager::getInstance() {
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
}

sf::Font& FontManager::getFont(FONT font) {
  return mFontMap[font];
}

void FontManager::clear() {
  mFontMap.clear();
}