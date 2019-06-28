#include "GuiButton.h"

#include <utility>
#include <SFML/Window/Mouse.hpp>

GuiButton::GuiButton(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback, int fontSize, FONT font)
  : GuiText(guiComponentPosition, std::move(text), fontSize, font), mCallback(callback) {
  mText.setOutlineThickness(1);

  if (font == FONT::MORRIS) {
    mYOffset = -5.0f;
  } else if (font == FONT::COURIER) {
    mYOffset = 5.0f;
  }

  mBackgroundL = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_LEFT);
  mBackgroundM = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON);
  mBackgroundR = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_RIGHT);

  mBackgroundLP = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_LEFT_PRESSED);
  mBackgroundMP = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_PRESSED);
  mBackgroundRP = GuiResourceManager::getInstance().createSprite(GUI_TEXTURE::BUTTON_RIGHT_PRESSED);

  mWidth = (int)(mBackgroundL.getLocalBounds().width + mText.getLocalBounds().width + mBackgroundR.getLocalBounds().width);
  mHeight = (int)mBackgroundM.getLocalBounds().height;
}

void GuiButton::setCallback(const std::function<void(void)>& callback) {
  mCallback = callback;
}

void GuiButton::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

  renderButton(window, xPosition, yPosition, width, height);
  calculatePosition(xPosition, yPosition, width, height);
  mText.setPosition((float)mXPosition + mBackgroundL.getLocalBounds().width, (float)mYPosition + mYOffset);
  if (mEnabled) {
    if (mIsSelected) {
      mText.setFillColor(sf::Color::Green);
    } else {
      mText.setFillColor(sf::Color::Black);
    }
  }

  //mBoundingBox.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);
  window->draw(mText);
}

bool GuiButton::checkMouse(sf::Vector2f mousePosition) {
  if (mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
      mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isEnabled()) {
      mCallback();
      return true;
    }
  }
  return false;
}

void GuiButton::renderButton(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

  if (mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
      mousePosition.y >= mBackgroundM.getPosition().y && mousePosition.y < mBackgroundM.getPosition().y + mBackgroundM.getLocalBounds().height) {
    mBackgroundLP.setPosition((float)xPosition, (float)yPosition);

    mBackgroundMP.setScale(sf::Vector2f((float)width / (float)mBackgroundMP.getTexture()->getSize().x - (mBackgroundLP.getLocalBounds().width + mBackgroundR.getLocalBounds().width), 1));
    mBackgroundMP.setPosition((float)xPosition + mBackgroundLP.getLocalBounds().width, (float)yPosition);

    mBackgroundRP.setPosition((float)xPosition + mBackgroundLP.getLocalBounds().width + mBackgroundMP.getLocalBounds().width * mBackgroundM.getScale().x, (float)yPosition);

    window->draw(mBackgroundLP);
    window->draw(mBackgroundMP);
    window->draw(mBackgroundRP);

    double time = animationClock.getElapsedTime().asMilliseconds();
    
    double timeThatFitInSin = time / (2000.0 / (3.14 * 2.0));  // Change the oscillation time to be 2000ms

    double s = sin(timeThatFitInSin - (3.14 / 2));  // Move the sine wave back PI/2 so we start at -1, since we add 1 in the next step

    double alpha = (s + 1) * 127.5;  // Add 1 so we don't get negative values, multiply by 127.5 so we get values between 0 and 255 (s+1 is between 0 and 2)

    mText.setOutlineColor(sf::Color(0, 120, 180, (int)alpha));

    mWidth = (int)(mBackgroundLP.getLocalBounds().width + mBackgroundMP.getScale().x + mBackgroundRP.getLocalBounds().width);
    mHeight = (int)mBackgroundMP.getLocalBounds().height;
  } else {
    animationClock.restart();

    mBackgroundL.setPosition((float)xPosition, (float)yPosition);

    mBackgroundM.setScale(sf::Vector2f((float)width / (float)mBackgroundM.getTexture()->getSize().x - (mBackgroundL.getLocalBounds().width + mBackgroundR.getLocalBounds().width), 1));
    mBackgroundM.setPosition((float)xPosition + mBackgroundL.getLocalBounds().width, (float)yPosition);

    mBackgroundR.setPosition((float)xPosition + mBackgroundL.getLocalBounds().width + mBackgroundM.getLocalBounds().width * mBackgroundM.getScale().x, (float)yPosition);

    window->draw(mBackgroundL);
    window->draw(mBackgroundM);
    window->draw(mBackgroundR);

    mText.setOutlineColor(sf::Color::Transparent);

    mWidth = (int)(mBackgroundL.getLocalBounds().width + mBackgroundM.getScale().x + mBackgroundR.getLocalBounds().width);
    mHeight = (int)mBackgroundM.getLocalBounds().height;
  }
}

void GuiButton::setEnabled() {
  mEnabled = true;
  mText.setFillColor(sf::Color::Black);
}

void GuiButton::setDisabled() {
  mEnabled = false;
  mText.setFillColor(sf::Color::Red);
}

bool GuiButton::isEnabled() {
  return mEnabled;
}

void GuiButton::select() {
  mIsSelected = true;
}

void GuiButton::unselect() {
  mIsSelected = false;
}

std::string GuiButton::getText() {
  return mText.getString();
}
