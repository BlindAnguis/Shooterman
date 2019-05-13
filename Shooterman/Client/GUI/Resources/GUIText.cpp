#include "GuiText.h"

GuiText::GuiText(GuiComponentPosition guiComponentPosition, std::string text) : GuiComponent(guiComponentPosition) {
  mName = "GuiButton";
  if (!mFont.loadFromFile("Client/Resources/Fonts/RobbieRocketpants.ttf")) {
    TRACE_ERROR("Could not load font");
    // TODO: ERROR
  }
  mText.setFont(mFont);
  mText.setString(text);
  mText.setCharacterSize(36);
  mText.setFillColor(sf::Color::Black);

  mWidth = (int)mText.getLocalBounds().width;
  mHeight = 36;
}

void GuiText::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  calculatePosition(xPosition, yPosition, width, height);
  mText.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);
  window->draw(mText);
}

void GuiText::setText(std::string newText) {
  mText.setString(newText);
}

void GuiText::setTextSize(int textSize) {
  mText.setCharacterSize(textSize);
  mWidth = (int)mText.getLocalBounds().width;
  mHeight = textSize;
}