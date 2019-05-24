#include "GuiButton.h"

#include <utility>
#include <SFML/Window/Mouse.hpp>

GuiButton::GuiButton(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback, int fontSize, FONT font)
  : GuiText(guiComponentPosition, std::move(text), fontSize, font), mCallback(callback) {
  mText.setOutlineThickness(1);

  mBackgroundL = FontManager::getInstance().createSprite(TEXTURE::BUTTON_LEFT);
  mBackgroundM = FontManager::getInstance().createSprite(TEXTURE::BUTTON);
  mBackgroundR = FontManager::getInstance().createSprite(TEXTURE::BUTTON_RIGHT);
}

void GuiButton::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

  renderButton(window, xPosition, yPosition, width, height);
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
}

bool GuiButton::checkMouse(sf::Vector2f mousePosition) {
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

void GuiButton::renderButton(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  mBackgroundL.setPosition((float)xPosition - (float)mBackgroundL.getLocalBounds().width, (float)yPosition + 5.0f);
  
  mBackgroundM.setScale(sf::Vector2f((float)width / (float)mBackgroundM.getTexture()->getSize().x, 1));
  mBackgroundM.setPosition((float)xPosition, (float)yPosition + 5.0f);

  mBackgroundR.setPosition((float)xPosition + (float)width, (float)yPosition + 5.0f);

  window->draw(mBackgroundL);
  window->draw(mBackgroundM);
  window->draw(mBackgroundR);
}