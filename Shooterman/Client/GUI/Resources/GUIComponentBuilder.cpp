#include "GUIComponentBuilder.h"

GUIComponent::GUIComponent(std::string textString, std::string textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, std::function<void(void)>& callback)
  : mTextColor(textColor), mTextHighlightColor(textHighlightColor), mCallback(callback) {
  if (!mFont.loadFromFile("Client/Resources/Fonts/" + textFont)) {
    TRACE_ERROR("Could not load file " << textFont);
  }
  mButtonText.setString(textString);
  mButtonText.setFont(mFont);
  mButtonText.setFillColor(textColor);
  mButtonText.setCharacterSize(textSize);
  float centeredXPosition = xPosition - mButtonText.getLocalBounds().width / 2;
  float centeredYPosition = yPosition - mButtonText.getLocalBounds().height / 2;
  mButtonText.setPosition(float(int(centeredXPosition)), float(int(centeredYPosition)));

  mBounds.setPosition(mButtonText.getPosition() + sf::Vector2f(0, mButtonText.getLocalBounds().height / 3));
  mBounds.setSize(sf::Vector2f(mButtonText.getLocalBounds().width, mButtonText.getLocalBounds().height));
  mBounds.setFillColor(sf::Color::Red);
}

bool GUIComponent::checkMouse(sf::Vector2f mousePosition) {
  if (mBounds.getGlobalBounds().contains(mousePosition)) {
    return true;
  }
  return false;
}

bool GUIComponent::isPressed(sf::Vector2f mousePosition) {
  if (checkMouse(mousePosition)) {
    if (mCallback) {
      mCallback();
    }
    return true;
  }
  return false;
}

void GUIComponent::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  //window->draw(mBounds);
  if (checkMouse(mousePosition)) {
    mButtonText.setFillColor(mTextHighlightColor);
  } else {
    mButtonText.setFillColor(mTextColor);
  }
  window->draw(mButtonText);
}

std::string GUIComponent::getText() {
  return mButtonText.getString();
}