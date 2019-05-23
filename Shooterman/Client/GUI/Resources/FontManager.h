#pragma once

#include <map>

#include <SFML/Graphics/Font.hpp>

#include "../../../Common/Trace.h"

enum FONT { CLEAN, DIRTY };

class FontManager : public Trace {
public:
  static FontManager& getInstance();
    
  sf::Font& getFont(FONT font);

  void clear();

  FontManager(FontManager const&) = delete;
  void operator=(FontManager const&) = delete;

private:
  std::map<FONT, sf::Font> mFontMap;

  FontManager();
};