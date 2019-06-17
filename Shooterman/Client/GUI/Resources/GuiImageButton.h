#pragma once

#include "GuiText.h"

#include <functional>

class GuiImageButton : public GuiText {
public:
  GuiImageButton(GuiComponentPosition guiComponentPosition, std::string text, sf::Sprite image, const std::function<void(void)>& callback = []() {}, int fontSize = 36, FONT font = FONT::MORRIS);

  void setCallback(const std::function<void(void)>& callback);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  bool checkMouse(sf::Vector2f mousePosition) override;

  void select();
  void unselect();

private:
  bool mIsSelected = false;
  std::function<void(void)> mCallback;
  sf::Sprite mImage;

  void renderImage(std::shared_ptr<sf::RenderWindow> window);
  void renderText(std::shared_ptr<sf::RenderWindow> window);
};