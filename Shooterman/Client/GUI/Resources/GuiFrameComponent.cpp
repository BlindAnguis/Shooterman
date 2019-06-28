#include "GuiFrameComponent.h"
#include <SFML/Window/Mouse.hpp>

GuiFrameComponent::GuiFrameComponent(GuiComponentPosition guiComponentPosition, sf::RectangleShape frameSize) :
  GuiComponent(guiComponentPosition),
  mFrameSize(frameSize)
{
  mFrameSize.setFillColor(sf::Color::Cyan);
  mFrameSize.setOutlineColor(sf::Color::Magenta);
  mFrameSize.setOutlineThickness(3.2f);
  mWidth = (int)mFrameSize.getSize().x;
  mHeight = (int)mFrameSize.getSize().y;
}

void GuiFrameComponent::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  calculateSize();
  calculatePosition(xPosition, yPosition, width, height);
  mFrameSize.setPosition(sf::Vector2f((float)mXPosition, (float)mYPosition));
  int xOffset = 0;
  int yOffset = 0;
  //mBoundingBox.setPosition((float)mXPosition, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);

  window->draw(mFrameSize);

  for (const auto &component : mGuiComponentList) {
    int allowedWith = (int)mFrameSize.getSize().x - mPadding;
    int allowedHeight = (int)mFrameSize.getSize().y - mPadding;
    component->render(window, mXPosition + mPadding, mYPosition + mPadding, allowedWith, allowedHeight);
  }
}

bool GuiFrameComponent::isMouseClickInsideFrame(sf::Vector2f mousePosition) {
  if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (!(mousePosition.x >= mXPosition && mousePosition.x < mXPosition + mWidth &&
      mousePosition.y >= mYPosition && mousePosition.y < mYPosition + mHeight)) {
      return false;
    }
  }
  return true;
}

bool GuiFrameComponent::checkMouse(sf::Vector2f mousePosition) {
  if (!isMouseClickInsideFrame(mousePosition)) {
    return false;
  }

  bool pressed = false;
  for (const auto &component : mGuiComponentList) {
    if (component->checkMouse(mousePosition)) {
      pressed = true;
    }
  }
  return pressed;
}

void GuiFrameComponent::addGuiComponent(std::shared_ptr<GuiComponent> guiComponent) {
  mGuiComponentList.emplace_back(guiComponent);
}

void GuiFrameComponent::removeGuiComponent(std::shared_ptr<GuiComponent> guiComponent) {
  mGuiComponentList.remove(guiComponent);
}

int GuiFrameComponent::getNumberOfComponents() {
  return mGuiComponentList.size();
}

std::shared_ptr<GuiComponent> GuiFrameComponent::getComponent(int index) {
  auto it = mGuiComponentList.begin();
  return *std::next(it, index);
}

void GuiFrameComponent::clear() {
  mGuiComponentList.clear();
}

void GuiFrameComponent::calculateSize() {
  for (const auto &component : mGuiComponentList) {
  }
}