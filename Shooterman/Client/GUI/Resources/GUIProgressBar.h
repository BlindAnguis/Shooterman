#pragma once

#include "GUIComponentBuilder.h"

class GUIProgressBar : public GUIComponent {
public:
  GUIProgressBar(float xPosition, float yPosition, float height, float width, sf::Color color);

  void render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;
  void setPosition(float xPosition, float yPosition) override;

  void setMaxValue(int value) override;
  void setCurrentValue(int value) override;

private:
  sf::RectangleShape mFrame;
  sf::RectangleShape mBar;
  float mMaxValue = 0;
  float mCurrentValue = 0;
};