#include "GuiInputText.h"

#include <SFML/Window/Mouse.hpp>

GuiInputText::GuiInputText(GuiComponentPosition guiComponentPosition, std::string text, const std::function<void(void)>& callback, int fontSize, FONT font)
  : GuiText(guiComponentPosition, text, fontSize, font), mCallback(callback) {
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

void GuiInputText::setText(std::string newText) {
  mText.setString(newText);
  mCurrentText = newText;
}

void GuiInputText::addChar(sf::Uint32 newChar) {
  if (!mReceiveInput) {
    // Not receiving input
    return;
  }
  if (newChar == 27) {
    // Esc key
    mReceiveInput = false;
    return;
  }
  if (newChar == 13) {
    // Enter key
    mCallback();
    return;
  }
  if (newChar == 8) {
    // Backspace
    mCurrentText = mCurrentText.substr(0, mCurrentText.size() - 1);
    return;
  }
  if (mCurrentText.length() > 15) {
    // String to long
    return;
  }
  mCurrentText += newChar;
}

void GuiInputText::enableReceiveInput() {
  mReceiveInputEnabled = true;
}

void GuiInputText::disableReceiveInput() {
  mReceiveInputEnabled = false;
}

std::string GuiInputText::getText() {
  return mCurrentText;
}
