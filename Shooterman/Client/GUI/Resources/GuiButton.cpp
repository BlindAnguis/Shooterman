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

  mBackgroundLP = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_LEFT_PRESSED);
  mBackgroundMP = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_PRESSED);
  mBackgroundRP = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_RIGHT_PRESSED);
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
}

bool GuiButton::checkMouse(sf::Vector2f mousePosition) {
  bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
  if (mousePosition.x >= mXPosition - mBackgroundL.getLocalBounds().width && mousePosition.x < mXPosition + mWidth + mBackgroundR.getLocalBounds().width &&
      mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight) {
    if (mousePressed && isEnabled()) {
      mCallback();
      return true;
    }
  }
  return false;
}

void GuiButton::renderButton(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

  if (mousePosition.x >= mBackgroundLP.getPosition().x && mousePosition.x < mBackgroundRP.getPosition().x + mBackgroundRP.getLocalBounds().width &&
      mousePosition.y >= mBackgroundMP.getPosition().y && mousePosition.y < mBackgroundMP.getPosition().y + mBackgroundMP.getLocalBounds().height) {
    mBackgroundLP.setPosition((float)xPosition - (float)mBackgroundLP.getLocalBounds().width, (float)yPosition + mYOffset);

    mBackgroundMP.setScale(sf::Vector2f((float)width / (float)mBackgroundMP.getTexture()->getSize().x, 1));
    mBackgroundMP.setPosition((float)xPosition, (float)yPosition + mYOffset);

    mBackgroundRP.setPosition((float)xPosition + (float)width, (float)yPosition + mYOffset);

    window->draw(mBackgroundLP);
    window->draw(mBackgroundMP);
    window->draw(mBackgroundRP);

    double time = animationClock.getElapsedTime().asMilliseconds();
    
    double timeThatFitInSin = time / (2000.0 / (3.14 * 2.0));  // Change the oscillation time to be 2000ms

    double s = sin(timeThatFitInSin - (3.14 / 2));  // Move the sine wave back PI/2 so we start at -1, since we add 1 in the next step

    int alpha = (s + 1) * 127.5;  // Add 1 so we don't get negative values, multiply by 127.5 so we get values between 0 and 255 (s+1 is between 0 and 2)

    mText.setOutlineColor(sf::Color(0, 120, 180, alpha));
  } else {
    animationClock.restart();

    mBackgroundL.setPosition((float)xPosition - (float)mBackgroundL.getLocalBounds().width, (float)yPosition + mYOffset);

    mBackgroundM.setScale(sf::Vector2f((float)width / (float)mBackgroundM.getTexture()->getSize().x, 1));
    mBackgroundM.setPosition((float)xPosition, (float)yPosition + mYOffset);

    mBackgroundR.setPosition((float)xPosition + (float)width, (float)yPosition + mYOffset);

    window->draw(mBackgroundL);
    window->draw(mBackgroundM);
    window->draw(mBackgroundR);

    mText.setOutlineColor(sf::Color::Transparent);
  }
}

void GuiButton::setEnabled() {
  enabled = true;
  mText.setFillColor(sf::Color::Black);
}

void GuiButton::setDisabled() {
  enabled = false;
  mText.setFillColor(sf::Color::Red);
}

bool GuiButton::isEnabled() {
  return enabled;
}