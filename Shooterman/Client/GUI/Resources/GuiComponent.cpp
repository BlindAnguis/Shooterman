#include "GuiComponent.h"

static int freeId = 0;

GuiComponent::GuiComponent() {
  mId = freeId;
  freeId++;
}

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
    mYPosition = yPosition + mPadding;
    break;
  case BOTTOM:
    mXPosition = (xPosition + (width / 2)) - (mWidth / 2);
    mYPosition = (yPosition + height - mPadding) - mHeight;
    break;
  case LEFT:
    mXPosition = xPosition + mPadding;
    mYPosition = (yPosition + (height / 2)) - (mHeight / 2);
    break;
  case RIGHT:
    mXPosition = (xPosition + width - mPadding) - mWidth;
    mYPosition = (yPosition + (height / 2)) - (mHeight / 2);
    break;
  case TOP_LEFT:
    mXPosition = xPosition + mPadding;
    mYPosition = yPosition + mPadding;
    break;
  case TOP_RIGHT:
    mXPosition = (xPosition + width - mPadding) - mWidth;
    mYPosition = yPosition + mPadding;
    break;
  case BOTTOM_LEFT:
    mXPosition = xPosition + mPadding;
    mYPosition = (yPosition + height - mPadding) - mHeight;
    break;
  case BOTTOM_RIGHT:
    mXPosition = (xPosition + width - mPadding) - mWidth;
    mYPosition = (yPosition + height - mPadding) - mHeight;
    break;
  }
}

int GuiComponent::getId() {
  return mId;
}