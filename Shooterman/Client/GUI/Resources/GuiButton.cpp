#include "GuiButton.h"

#include <utility>
#include <SFML/Window/Mouse.hpp>

GuiButton::GuiButton(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback, int fontSize, FONT font)
  : GuiText(guiComponentPosition, std::move(text), fontSize, font), mCallback(callback) {
  mText.setOutlineThickness(1);

  if (font == FONT::MORRIS) {
    mYOffset = 5.0f;
  } else if (font == FONT::COURIER) {
    mYOffset = -5.0f;
  }

  mBackgroundL = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_LEFT);
  mBackgroundM = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON);
  mBackgroundR = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_RIGHT);
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
    if (mousePressed && isEnabled()) {
      mCallback();
      return true;
    }
  }
  return false;
}

void GuiButton::renderButton(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  mBackgroundL.setPosition((float)xPosition - (float)mBackgroundL.getLocalBounds().width, (float)yPosition + mYOffset);
  
  mBackgroundM.setScale(sf::Vector2f((float)width / (float)mBackgroundM.getTexture()->getSize().x, 1));
  mBackgroundM.setPosition((float)xPosition, (float)yPosition + mYOffset);

  mBackgroundR.setPosition((float)xPosition + (float)width, (float)yPosition + mYOffset);

  window->draw(mBackgroundL);
  window->draw(mBackgroundM);
  window->draw(mBackgroundR);
}

void GuiButton::setEnabled() {
  this->enabled = true;
}

void GuiButton::setDisabled() {
  this->enabled = false;
}

bool GuiButton::isEnabled() {
  return this->enabled;
}