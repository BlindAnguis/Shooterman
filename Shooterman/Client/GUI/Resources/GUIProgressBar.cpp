#include "GUIProgressBar.h"

GUIProgressBar::GUIProgressBar(float xPosition, float yPosition, float height, float width, sf::Color color) {
  mFrame.setFillColor(sf::Color::Black);
  mFrame.setOutlineColor(sf::Color::Black);
  mFrame.setOutlineThickness(1);
  mFrame.setPosition(xPosition, yPosition);
  mFrame.setSize(sf::Vector2f(height, width));

  mBar.setFillColor(color);
  mBar.setOutlineColor(sf::Color::Black);
  mBar.setPosition(xPosition, yPosition);
  mBar.setSize(sf::Vector2f(height, width));
}

void GUIProgressBar::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {

  window->draw(mFrame);

  float percentage = mCurrentValue / mMaxValue;
  mBar.setSize(sf::Vector2f(mFrame.getSize().x * percentage, mFrame.getSize().y));
  window->draw(mBar);
}

void GUIProgressBar::setPosition(float xPosition, float yPosition) {
  mFrame.setPosition(xPosition, yPosition);
  mBar.setPosition(xPosition, yPosition);
}

void GUIProgressBar::setMaxValue(int value) {
  mMaxValue = (float) value;
}

void GUIProgressBar::setCurrentValue(int value) {
  mCurrentValue = (float) value;
}