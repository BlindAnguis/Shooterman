#pragma once

#include "GuiText.h"

#include <functional>

class GuiButton : public GuiText {
public:
  GuiButton(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  bool checkMouse(sf::Vector2f mousePosition) override;

private:
  std::function<void(void)> mCallback;
};