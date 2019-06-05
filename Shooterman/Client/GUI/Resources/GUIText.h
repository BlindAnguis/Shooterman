#pragma once

#include <SFML/Graphics/Text.hpp>

#include "GuiComponent.h"
#include "GuiResourceManager.h"

#include "../../../Common/Trace.h"

class GuiText : public GuiComponent, Trace {
public:
  GuiText(GuiComponentPosition guiComponentPosition, std::string text, int fontSize = 36, FONT font = FONT::MORRIS);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  bool checkMouse(sf::Vector2f mousePosition) override;
  
  void setText(std::string newText);
  void addChar(sf::Uint32 newChar);
  void removeChar();

  void enableReceiveInput();
  void disableReceiveInput();

  void setPosition(float xPosition, float yPosition);
  void setOutlineColor(sf::Color newColor);

protected:
  bool mReceiveInputEnabled = false;
  bool mReceiveInput = false;
  bool mFirstClick = true;
  sf::Text mText;
  std::string mDefaultText;
};