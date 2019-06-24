#include "GuiInputText.h"

#include <SFML/Window/Mouse.hpp>

GuiInputText::GuiInputText(GuiComponentPosition guiComponentPosition, std::string text, int fontSize, FONT font)
  : GuiText(guiComponentPosition, text, fontSize, font) {
  mName = "GuiInputText";
  mDefaultText = text;
  mText.setString("XXXXXXXXXXXXXXX");
  mWidth = (int)mText.getLocalBounds().width;
}

void GuiInputText::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  if (mReceiveInput) {
    if (mCursorEnabled) {
      mText.setString(mCurrentText + "|");
    } else {
      mText.setString(mCurrentText);
    }

    if (mCursorClock.getElapsedTime() >= sf::milliseconds(500)) {
      mCursorEnabled = !mCursorEnabled;
      mCursorClock.restart();
    }
  } else {
    if (mCurrentText.size() == 0) {
      mText.setString(mDefaultText);
    } else {
      mText.setString(mCurrentText);
    }
  }

  calculatePosition(xPosition, yPosition, width, height);
  mText.setPosition((float)mXPosition, (float)mYPosition);

  window->draw(mText);
}

bool GuiInputText::checkMouse(sf::Vector2f mousePosition) {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
      mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight) {
      if (mReceiveInputEnabled) {
        mReceiveInput = true;
      }
      return true;
    } else if (mReceiveInputEnabled) {
      mReceiveInput = false;
    }
  }
  return false;
}

void GuiInputText::addChar(sf::Uint32 newChar) {
  if (mReceiveInput) {
    mCurrentText += newChar;
  }
}

void GuiInputText::removeChar() {
  if (mReceiveInput) {
    mCurrentText = mCurrentText.substr(0, mCurrentText.size() - 1);
  }
}

void GuiInputText::enableReceiveInput() {
  mReceiveInputEnabled = true;
}

void GuiInputText::disableReceiveInput() {
  mReceiveInputEnabled = false;
}