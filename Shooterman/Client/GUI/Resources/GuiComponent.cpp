#include "GuiComponent.h"

GuiComponent::GuiComponent() {}

GuiComponent::GuiComponent(GuiComponentPosition guiComponentPosition)
  : mGuiComponentPosition(guiComponentPosition) {
  mBoundingBox.setOutlineThickness(1);
  mBoundingBox.setOutlineColor(sf::Color::Magenta);
  mBoundingBox.setFillColor(sf::Color::Transparent);
}

bool GuiComponent::checkMouse(sf::Vector2f mousePosition) { return false; }

int GuiComponent::getXPosition() {
  return mXPosition;
}

int GuiComponent::getYPosition() {
  return mYPosition;
}

int GuiComponent::getWidth() {
  return mWidth;
}

int GuiComponent::getHeight() {
  return mHeight;
}

void GuiComponent::calculatePosition(int xPosition, int yPosition, int width, int height) {
  switch (mGuiComponentPosition) {
  case CENTER:
    mXPosition = (xPosition + (width / 2)) - (mWidth / 2);
    mYPosition = (yPosition + (height / 2)) - (mHeight / 2);
    break;
  case TOP:
    mXPosition = (xPosition + (width / 2)) - (mWidth / 2);
    mYPosition = yPosition;
    break;
  case BOTTOM:
    mXPosition = (xPosition + (width / 2)) - (mWidth / 2);
    mYPosition = (yPosition + height) - mHeight;
    break;
  case LEFT:
    mXPosition = xPosition;
    mYPosition = (yPosition + (height / 2)) - (mHeight / 2);
    break;
  case RIGHT:
    mXPosition = (xPosition + width) - mWidth;
    mYPosition = (yPosition + (height / 2)) - (mHeight / 2);
    break;
  case TOP_LEFT:
    mXPosition = xPosition;
    mYPosition = yPosition;
    break;
  case TOP_RIGHT:
    mXPosition = (xPosition + width) - mWidth;
    mYPosition = yPosition;
    break;
  case BOTTOM_LEFT:
    mXPosition = xPosition;
    mYPosition = (yPosition + height) - mHeight;
    break;
  case BOTTOM_RIGHT:
    mXPosition = (xPosition + width) - mWidth;
    mYPosition = (yPosition + height) - mHeight;
    break;
  }
}