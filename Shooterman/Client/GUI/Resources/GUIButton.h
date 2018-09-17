#pragma once

#include <SFML/Graphics.hpp>

#include "../../../Common/Trace.h"

enum BUTTON_PRESSED { NO_ACTION, B_MAIN_MENU, B_LOBBY, B_START, B_JOIN, B_BACK, B_QUIT };

class GUIButton : Trace {
public:
  GUIButton(std::string textString, sf::Font& textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, BUTTON_PRESSED buttonPressedAction);
  ~GUIButton() {
    delete mButtonText;
  }

  BUTTON_PRESSED checkMouse(sf::Vector2f mousePosition);

  void render(sf::RenderWindow* window);

private:
  sf::Text* mButtonText;
  sf::RectangleShape mBounds;
  sf::Color mTextColor;
  sf::Color mTextHighlightColor;
  BUTTON_PRESSED mButtonPressedAction;
};