#include "GuiList.h"

GuiList::GuiList(GuiComponentPosition guiComponentPosition, GuiListDirection guiListDirection) :
  GuiComponent(guiComponentPosition),
  mGuiListDirection(guiListDirection) {}

void GuiList::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  calculateSize();
  calculatePosition(xPosition, yPosition, width, height);
  int xOffset = 0;
  int yOffset = 0;
  //mBoundingBox.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);
  for (const auto &component : mGuiComponentList) {
    int allowedWith = 0;
    int allowedHeight = 0;

    if (mGuiListDirection == GuiListDirection::HORIZONTAL) {
      allowedWith = component->getWidth();
      allowedHeight = mHeight;
    } else {
      allowedWith = mWidth;
      allowedHeight = component->getHeight();
    }

    component->render(window, mXPosition + xOffset, mYPosition + yOffset, allowedWith, allowedHeight);
    switch (mGuiListDirection) {
    case HORIZONTAL:
      xOffset += component->getWidth();
      xOffset += padding;
      break;
    case VERTICAL:
      yOffset += component->getHeight();
      yOffset += padding;
      break;
    }
  }
}

bool GuiList::checkMouse(sf::Vector2f mousePosition) {
  for (const auto &component : mGuiComponentList) {
    component->checkMouse(mousePosition);
  }
  return false;
}

void GuiList::addGuiComponent(std::shared_ptr<GuiComponent> guiComponent) {
  mGuiComponentList.emplace_back(guiComponent);
  calculateSize();
}

void GuiList::calculateSize() {
  mWidth = 0;
  mHeight = 0;
  for (const auto &component : mGuiComponentList) {
    if (mGuiListDirection == GuiListDirection::HORIZONTAL) {
      mWidth += component->getWidth();
      mWidth += padding;

      if (component->getHeight() > mHeight) {
        mHeight = component->getHeight();
      }
    } else {
      mHeight += component->getHeight();
      mHeight += padding;

      if (component->getWidth() > mWidth) {
        mWidth = component->getWidth();
      }
    }
  }
  if (mGuiListDirection == GuiListDirection::HORIZONTAL) {
    mWidth -= padding;
  } else {
    mHeight -= padding;
  }
}