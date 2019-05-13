#pragma once

#include <SFML/Graphics/Text.hpp>

#include "GuiComponent.h"

class GuiProgressBar : public GuiComponent {
public:
  GuiProgressBar(GuiComponentPosition guiComponentPosition);
  GuiProgressBar(float xPosition, float yPosition, float height, float width, sf::Color color);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  void setPosition(float xPosition, float yPosition);

  void setMaxValue(int value);
  void setCurrentValue(int value);

protected:
  sf::RectangleShape mFrame;
  sf::RectangleShape mBar;
  float mMaxValue = 0;
  float mCurrentValue = 0;
};
