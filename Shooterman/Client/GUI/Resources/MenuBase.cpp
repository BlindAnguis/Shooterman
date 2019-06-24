#include "MenuBase.h"

MenuBase::~MenuBase() { }

void MenuBase::init() { }
void MenuBase::uninit() { }
void MenuBase::reset() { }

bool MenuBase::checkMouse(sf::Vector2f mousePosition) {
  return mGuiFrame->checkMouse(mousePosition);
}

bool MenuBase::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  mGuiFrame->render(window);
  return true;
}

void MenuBase::handleNewText(sf::Uint32 newChar) { }