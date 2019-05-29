#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "GuiComponent.h"

class Frame {
public:
  explicit Frame();

  void addGuiComponent(std::shared_ptr<GuiComponent> component);
  void render(std::shared_ptr<sf::RenderWindow> window);
  bool checkMouse(sf::Vector2f mousePosition);
  void clearGuiComponents();

private:
  std::vector<std::shared_ptr<GuiComponent>> mComponents;
};