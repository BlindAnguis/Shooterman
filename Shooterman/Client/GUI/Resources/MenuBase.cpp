#include "MenuBase.h"


bool MenuBase::checkMouse(sf::Vector2f mousePosition) {
  for (GUIButton* button : mButtonList) {
    if (button->isPressed(mousePosition)) {
      return true;
    }
  }
  return false;
}

void MenuBase::render(sf::RenderWindow* window, sf::Vector2f mousePosition) {
  for (GUIButton* button : mButtonList) {
    button->render(window, mousePosition);
  }
}

void MenuBase::loadFont(std::string fontName) {
  if (!mFont.loadFromFile(FONT_FILE_PATH + fontName)) {
    TRACE_ERROR("Could not load file " << fontName);
  }
}