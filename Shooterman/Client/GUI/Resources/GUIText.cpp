#include "GuiText.h"

#include <SFML/Window/Mouse.hpp>

GuiText::GuiText(GuiComponentPosition guiComponentPosition, std::string text, int fontSize, FONT font) : GuiComponent(guiComponentPosition) {
  mName = "GuiText";
  mText.setFont(GuiResourceManager::getInstance().getFont(font));
  mText.setString(text);
  mText.setCharacterSize(fontSize);
  mText.setFillColor(sf::Color::Black);

  mWidth = (int)mText.getLocalBounds().width;
  mHeight = fontSize;
}

void GuiText::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  calculatePosition(xPosition, yPosition, width, height);
  mText.setPosition((float)mXPosition, (float)mYPosition);
  mWidth = (int)mText.getLocalBounds().width;

  //mBoundingBox.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);
  window->draw(mText);
}

void GuiText::setText(std::string newText) {
  mText.setString(newText);
}

void GuiText::setPosition(float xPosition, float yPosition) {
  mXFixedPosition = xPosition;
  mYFixedPosition = yPosition;
}

void GuiText::setOutlineColor(sf::Color newColor) {
  mText.setFillColor(sf::Color::White);
  mText.setOutlineColor(newColor);
  mText.setOutlineThickness(1);
}