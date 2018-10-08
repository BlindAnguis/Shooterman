#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../../../Common/Trace.h"

class GUIButton : Trace {
public:
  GUIButton(std::string textString, sf::Font& textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, std::function<void(void)> callback = []() {});

  ~GUIButton() { }

  bool isPressed(sf::Vector2f mousePosition);
  void render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition);

private:
  sf::Text mButtonText;
  sf::RectangleShape mBounds;
  sf::Color mTextColor;
  sf::Color mTextHighlightColor;
  std::function<void(void)> mCallback;

  bool checkMouse(sf::Vector2f mousePosition);
};