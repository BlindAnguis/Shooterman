#pragma once

#include <SFML/Graphics/Text.hpp>

#include "GuiComponent.h"

#include "../../../Common/Trace.h"

class GuiText : public GuiComponent, Trace {
public:
  GuiText(GuiComponentPosition guiComponentPosition, std::string text);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;

  void setText(std::string newText);
  void setTextSize(int textSize);

protected:
  sf::Font mFont;
  sf::Text mText;
};