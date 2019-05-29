#pragma once

#include "GuiText.h"

#include <functional>

class GuiButton : public GuiText {
public:
  GuiButton(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback, int fontSize = 36, FONT font = FONT::MORRIS);
  void setEnabled();
  void setDisabled();
  bool isEnabled();
  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  bool checkMouse(sf::Vector2f mousePosition) override;

private:
  std::function<void(void)> mCallback;
  sf::Sprite mBackgroundL;
  sf::Sprite mBackgroundM;
  sf::Sprite mBackgroundR;
  float mYOffset = 0;
  bool enabled = true;

  void renderButton(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height);
};