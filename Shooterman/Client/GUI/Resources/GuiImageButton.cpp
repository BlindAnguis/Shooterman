#include "GuiImageButton.h"

#include <SFML/Window/Mouse.hpp>

GuiImageButton::GuiImageButton(GuiComponentPosition guiComponentPosition, std::string text, sf::Sprite image, const std::function<void(void)>& callback, int fontSize, FONT font) :
  GuiText(guiComponentPosition, std::move(text), fontSize, font), mImage(image), mCallback(callback) {

  mWidth = (int)std::max(mImage.getLocalBounds().width, mText.getLocalBounds().width);
  mHeight = (int)mImage.getLocalBounds().height + mText.getCharacterSize() + 10;
}

void GuiImageButton::setCallback(const std::function<void(void)>& callback) {
  mCallback = callback;
}

void GuiImageButton::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  calculatePosition(xPosition, yPosition, width, height);

  renderImage(window);
  renderText(window);

  //mBoundingBox.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);
}

bool GuiImageButton::checkMouse(sf::Vector2f mousePosition) {
  bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
  if (mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
    mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight) {
    if (mousePressed) {
      mCallback();
      return true;
    }
  }
  return false;
}

void GuiImageButton::select() {
  mIsSelected = true;
}

void GuiImageButton::unselect() {
  mIsSelected = false;
}

void GuiImageButton::renderImage(std::shared_ptr<sf::RenderWindow> window) {
  float boundingBoxXCenter = (float)(mXPosition + (mWidth / 2));
  mImage.setPosition(boundingBoxXCenter - (mImage.getLocalBounds().width / 2), (float)mYPosition);
  window->draw(mImage);
}

void GuiImageButton::renderText(std::shared_ptr<sf::RenderWindow> window) {
  float boundingBoxXCenter = (float)(mXPosition + (mWidth / 2));
  mText.setPosition(boundingBoxXCenter - (mText.getLocalBounds().width / 2), mYPosition + mImage.getLocalBounds().height + 10);

  if (mIsSelected) {
    mText.setFillColor(sf::Color::Green);
  } else {
    mText.setFillColor(sf::Color::Black);
  }

  window->draw(mText);
}