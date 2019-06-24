#pragma once

#include <map>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../../../Common/Textures.h"
#include "../../../Common/Trace.h"

enum FONT { COURIER, MORRIS };
enum GUI_TEXTURE { BUTTON, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_PRESSED, BUTTON_LEFT_PRESSED, BUTTON_RIGHT_PRESSED };

class GuiResourceManager : public Trace {
public:
  static GuiResourceManager& getInstance();
    
  sf::Font& getFont(FONT font);
  sf::Sprite createSprite(GUI_TEXTURE texture);
  sf::Sprite createSprite(Textures texture);

  void clear();

  GuiResourceManager(GuiResourceManager const&) = delete;
  void operator=(GuiResourceManager const&) = delete;

private:
  std::map<FONT, sf::Font> mFontMap;
  std::map<GUI_TEXTURE, sf::Texture> mGuiTextureMap;
  std::map<Textures, sf::Texture> mSpriteTextureMap;
  std::map<Textures, sf::IntRect> mSpriteTextureRectMap;

  GuiResourceManager();
  ~GuiResourceManager();

  void loadFont(FONT fontType, std::string fileName);
  void loadGuiTexture(GUI_TEXTURE guiTexture, std::string fileName);
  void loadSpriteTexture(Textures spriteTexture, std::string fileName);
};