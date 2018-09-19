#include "GUIButton.h"

GUIButton::GUIButton(std::string textString, sf::Font& textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, std::function<void(void)> callback)
  : mTextColor(textColor), mTextHighlightColor(textHighlightColor), mCallback(callback) {
  mButtonText.setString(textString);
  mButtonText.setFont(textFont);
  mButtonText.setFillColor(textColor);
  mButtonText.setCharacterSize(textSize);
  float centeredXPosition = xPosition - mButtonText.getLocalBounds().width / 2;
  float centeredYPosition = yPosition - mButtonText.getLocalBounds().height / 2;
  mButtonText.setPosition(float(int(centeredXPosition)), float(int(centeredYPosition)));

  mBounds.setPosition(mButtonText.getPosition() + sf::Vector2f(0, mButtonText.getLocalBounds().height / 3));
  mBounds.setSize(sf::Vector2f(mButtonText.getLocalBounds().width, mButtonText.getLocalBounds().height));
  mBounds.setFillColor(sf::Color::Red);
}

bool GUIButton::checkMouse(sf::Vector2f mousePosition) {
  if (mBounds.getGlobalBounds().contains(mousePosition)) {
    return true;
  }
  return false;
}

bool GUIButton::isPressed(sf::Vector2f mousePosition) {
  if (checkMouse(mousePosition)) {
    mCallback();
    return true;
  }
  return false;
}

void GUIButton::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  //window->draw(mBounds);
  if (checkMouse(mousePosition)) {
    mButtonText.setFillColor(mTextHighlightColor);
  } else {
    mButtonText.setFillColor(mTextColor);
  }
  window->draw(mButtonText);
}