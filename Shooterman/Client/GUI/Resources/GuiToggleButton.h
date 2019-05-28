#pragma once

#include "GuiText.h"

#include <functional>

class GuiToggleButton : public GuiText {
public:
  GuiToggleButton(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback, int fontSize = 36, FONT font = FONT::MORRIS);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  bool checkMouse(sf::Vector2f mousePosition) override;

private:
  bool mActive = false;
  std::function<void(void)> mCallback;
};
