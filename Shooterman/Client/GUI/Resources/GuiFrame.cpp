#include "GuiFrame.h"

Frame::Frame() = default;

void Frame::addGuiComponent(std::shared_ptr<GuiComponent> component) {
  mComponents.emplace_back(component);
}

void Frame::render(std::shared_ptr<sf::RenderWindow> window) {
  for (const auto &component : mComponents) {
    component->render(window, 0, 0, window->getSize().x, window->getSize().y);
  }
}

bool Frame::checkMouse(sf::Vector2f mousePosition) {
  for (const auto &component : mComponents) {
    component->checkMouse(mousePosition);
  }
  return false;
}