#include "GuiText.h"

#include <SFML/Window/Mouse.hpp>

GuiText::GuiText(GuiComponentPosition guiComponentPosition, std::string text, int fontSize, FONT font) : GuiComponent(guiComponentPosition) {
  mName = "GuiButton";
  mDefaultText = text;
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

bool GuiText::checkMouse(sf::Vector2f mousePosition) {
  bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
  if (mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
    mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight) {
    if (mousePressed) {
      if (mReceiveInputEnabled) {
        mReceiveInput = true;
        if (mFirstClick) {
          mText.setString("");
        }
        mFirstClick = false;
      }
      return true;
    }
  }
  if (mReceiveInputEnabled) {
    mReceiveInput = false;
    if (!mFirstClick && mText.getString().getSize() == 0) {
      mText.setString(mDefaultText);
      mFirstClick = true;
    }
  }
  return false;
}

void GuiText::setText(std::string newText) {
  mText.setString(newText);
}

void GuiText::addChar(sf::Uint32 newChar) {
  if (mReceiveInput) {
    mText.setString(mText.getString() + newChar);
  }
}

void GuiText::removeChar() {
  if (mReceiveInput) {
    mText.setString(mText.getString().substring(0, mText.getString().getSize() - 1));
  }
}

void GuiText::enableReceiveInput() {
  mReceiveInputEnabled = true;
}

void GuiText::disableReceiveInput() {
  mReceiveInputEnabled = false;
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