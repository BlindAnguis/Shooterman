#pragma once

#include <SFML/Graphics/Text.hpp>

#include "GuiComponent.h"
#include "GuiResourceManager.h"

#include "../../../Common/Trace.h"

class GuiText : public GuiComponent, public Trace {
public:
  GuiText(GuiComponentPosition guiComponentPosition, std::string text, int fontSize = 36, FONT font = FONT::MORRIS);

  virtual void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  
  virtual void setText(std::string newText);

  void setPosition(float xPosition, float yPosition);
  void setOutlineColor(sf::Color newColor);
  std::string getText();

protected:
  sf::Text mText;
};