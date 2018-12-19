#include "MenuBase.h"

bool MenuBase::checkMouse(sf::Vector2f mousePosition) {
  for (auto component : mComponentList) {
    if (component->isPressed(mousePosition)) {
      return true;
    }
  }
  return false;
}

bool MenuBase::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  for (auto component : mComponentList) {
    component->render(window, mousePosition);
  }
  return true;
}