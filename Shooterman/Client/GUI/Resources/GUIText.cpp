#include "GUIText.h"

GUIText::GUIText(std::string textString, std::string textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, std::function<void(void)>& callback, bool toggleCollorOnClick)
  : mTextColor(textColor), mTextHighlightColor(textHighlightColor), mCallback(callback), mToggleColorOnClick(toggleCollorOnClick) {
  if (!mFont.loadFromFile("Client/Resources/Fonts/" + textFont)) {
    TRACE_ERROR("Could not load file " << textFont);
  }
  mComponentText.setString(textString);
  mComponentText.setFont(mFont);
  mComponentText.setFillColor(textColor);
  mComponentText.setOutlineColor(mTextHighlightColor);
  mComponentText.setCharacterSize(textSize);
  float centeredXPosition = xPosition - mComponentText.getLocalBounds().width / 2;
  float centeredYPosition = yPosition - mComponentText.getLocalBounds().height / 2;
  mComponentText.setPosition(float(int(centeredXPosition)), float(int(centeredYPosition)));

  mBounds.setPosition(mComponentText.getPosition() + sf::Vector2f(0, mComponentText.getLocalBounds().height / 3));
  mBounds.setSize(sf::Vector2f(mComponentText.getLocalBounds().width, mComponentText.getLocalBounds().height));
  mBounds.setFillColor(sf::Color::White);
}

bool GUIText::checkMouse(sf::Vector2f mousePosition) {
  if (mBounds.getGlobalBounds().contains(mousePosition)) {
    return true;
  }
  return false;
}

bool GUIText::isPressed(sf::Vector2f mousePosition) {
  if (checkMouse(mousePosition)) {
    if (mCallback) {
      if (mToggleColorOnClick) {
        if (!hasBeenClicked) {
          mComponentText.setFillColor(mClickedColor);
          hasBeenClicked = true;
        } else {
          mComponentText.setFillColor(mTextColor);
          hasBeenClicked = false;
        }
      }
      mCallback();
    }
    return true;
  }
  return false;
}

void GUIText::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  if (mRenderBounds) {
    window->draw(mBounds);
  }
  if (checkMouse(mousePosition)) {
    mComponentText.setOutlineThickness(1);
  } else {
    mComponentText.setOutlineThickness(0);
  }
  window->draw(mComponentText);
}

std::string GUIText::getText() {
  return mComponentText.getString();
}

void GUIText::setText(std::string newText) {
  mComponentText.setString(newText);
  mBounds.setPosition(mComponentText.getPosition() + sf::Vector2f(0, mComponentText.getLocalBounds().height / 3));
  mBounds.setSize(sf::Vector2f(mComponentText.getLocalBounds().width, mComponentText.getLocalBounds().height));
}

void GUIText::setRenderBounds(bool renderBounds) {
  mRenderBounds = renderBounds;
}

int GUIText::getWidth() {
  return (int)mComponentText.getLocalBounds().width;
}

int GUIText::getHeight() {
  return (int)mComponentText.getLocalBounds().height;
}