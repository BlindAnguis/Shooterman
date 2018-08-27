#include "GUIButton.h"

GUIButton::GUIButton(std::string textString, sf::Font& textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, BUTTON_PRESSED buttonPressedAction)
  : mTextColor(textColor), mTextHighlightColor(textHighlightColor), mButtonPressedAction(buttonPressedAction) {
  mButtonText = new sf::Text();
  mButtonText->setString(textString);
  mButtonText->setFont(textFont);
  mButtonText->setFillColor(textColor);
  mButtonText->setCharacterSize(textSize);
  float centeredXPosition = xPosition - mButtonText->getLocalBounds().width / 2;
  float centeredYPosition = yPosition - mButtonText->getLocalBounds().height / 2;
  mButtonText->setPosition(float(int(centeredXPosition)), float(int(centeredYPosition)));

  mBounds.setPosition(mButtonText->getPosition() + sf::Vector2f(0, mButtonText->getLocalBounds().height / 3));
  mBounds.setSize(sf::Vector2f(mButtonText->getLocalBounds().width, mButtonText->getLocalBounds().height));
  mBounds.setFillColor(sf::Color::Red);
}

BUTTON_PRESSED GUIButton::checkMouse(sf::Vector2f mousePosition) {
  if (mBounds.getGlobalBounds().contains(mousePosition)) {
    mButtonText->setFillColor(mTextHighlightColor);
    return mButtonPressedAction;
  }
  mButtonText->setFillColor(mTextColor);
  return NO_ACTION;
}

void GUIButton::render(sf::RenderWindow* window) {
  //window->draw(mBounds);
  window->draw(*mButtonText);
}