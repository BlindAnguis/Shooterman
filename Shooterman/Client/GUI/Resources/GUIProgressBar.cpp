#include "GuiProgressBar.h"

GuiProgressBar::GuiProgressBar(GuiComponentPosition guiComponentPosition) {}

GuiProgressBar::GuiProgressBar(float xPosition, float yPosition, float height, float width, sf::Color color) {
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


void GuiProgressBar::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  window->draw(mFrame);

  float percentage = mCurrentValue / mMaxValue;
  mBar.setSize(sf::Vector2f(mFrame.getSize().x * percentage, mFrame.getSize().y));
  window->draw(mBar);
}

void GuiProgressBar::setPosition(float xPosition, float yPosition) {
  mFrame.setPosition(xPosition, yPosition);
  mBar.setPosition(xPosition, yPosition);
}

void GuiProgressBar::setMaxValue(int value) {
  mMaxValue = (float)value;
}

void GuiProgressBar::setCurrentValue(int value) {
  mCurrentValue = (float)value;
  if (mCurrentValue < 0) {
    mCurrentValue = 0;
  }
}