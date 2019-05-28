#include "GuiToggleButton.h"

#include <utility>
#include <SFML/Window/Mouse.hpp>

GuiToggleButton::GuiToggleButton(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback, int fontSize, FONT font)
  : GuiText(guiComponentPosition, std::move(text), fontSize, font), mCallback(callback) {
  mText.setOutlineThickness(1);
  mText.setFillColor(sf::Color::Red);
}

void GuiToggleButton::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

  calculatePosition(xPosition, yPosition, width, height);
  mText.setPosition((float)mXPosition, (float)mYPosition);

  //mBoundingBox.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);
  window->draw(mText);

  if (mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
    mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight) {
    mText.setOutlineColor(sf::Color(0, 120, 180));
  } else {
    mText.setOutlineColor(sf::Color::Transparent);
  }

  if (mActive) {
    mText.setFillColor(sf::Color(0, 150, 0));
  } else {
    mText.setFillColor(sf::Color(150, 0, 0));
  }
}

bool GuiToggleButton::checkMouse(sf::Vector2f mousePosition) {
  bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
  if (mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
    mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight) {
    if (mousePressed) {
      mActive = !mActive;
      mCallback();
      return true;
    }
  }
  return false;
}