#pragma once

#include <SFML/Graphics/Text.hpp>

#include "GuiComponent.h"
#include "GuiResourceManager.h"

#include "../../../Common/Trace.h"

class GuiText : public GuiComponent, Trace {
public:
  GuiText(GuiComponentPosition guiComponentPosition, std::string text, int fontSize = 36, FONT font = FONT::MORRIS);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;

  void setText(std::string newText);
  void setTextSize(int textSize);

protected:
  sf::Text mText;
};