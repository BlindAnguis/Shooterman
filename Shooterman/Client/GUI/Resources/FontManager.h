#pragma once

#include <map>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../../../Common/Trace.h"

enum FONT { COURIER, MORRIS };
enum TEXTURE { BUTTON, BUTTON_LEFT, BUTTON_RIGHT };

class FontManager : public Trace {
public:
  static FontManager& getInstance();
    
  sf::Font& getFont(FONT font);
  sf::Sprite createSprite(TEXTURE texture);

  void clear();

  FontManager(FontManager const&) = delete;
  void operator=(FontManager const&) = delete;

private:
  std::map<FONT, sf::Font> mFontMap;
  std::map<TEXTURE, sf::Texture> mTextureMap;

  FontManager();
};