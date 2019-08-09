#include "MenuBase.h"

MenuBase::~MenuBase() { }

void MenuBase::init() { }
void MenuBase::uninit() { }
void MenuBase::reset() { }
void MenuBase::backgroundUpdate() { }

bool MenuBase::checkMouse(sf::Vector2f mousePosition) {
  return mGuiFrame->checkMouse(mousePosition);
}

bool MenuBase::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  mGuiFrame->render(window);
  return true;
}

void MenuBase::handleNewText(sf::Uint32 newChar) {
  for (auto textListener : mTextListeners) {
    textListener->addChar(newChar);
  }
}

void MenuBase::addTextListener(std::shared_ptr<GuiInputText> textListener) {
  mTextListeners.push_back(textListener);
}