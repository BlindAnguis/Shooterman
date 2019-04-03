#pragma once

#include "GuiComponent.h"

class GUIText : public GUIComponent {
public:
  GUIText(std::string textString, std::string textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, std::function<void(void)>& callback, bool toggleCollorOnClick);

  bool isPressed(sf::Vector2f mousePosition) override;
  void render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;
  std::string getText() override;
  void setText(std::string newText) override;
  void setRenderBounds(bool renderBounds) override;
  int getWidth() override;
  int getHeight() override;

private:
  sf::Text mComponentText;
  sf::Font mFont;
  sf::RectangleShape mBounds;
  sf::Color mTextColor;
  sf::Color mTextHighlightColor;
  sf::Color mClickedColor = sf::Color::Blue;
  std::function<void(void)> mCallback;
  bool mRenderBounds = false;
  bool mToggleColorOnClick;
  bool hasBeenClicked = false;

  bool checkMouse(sf::Vector2f mousePosition);
};