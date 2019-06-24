#include "GuiExpandableList.h"

#include "GuiComponentFactory.h"

GuiExpandableList::GuiExpandableList(GuiComponentPosition guiComponentPosition, std::string header, FONT font)
  : GuiList(guiComponentPosition, GuiListDirection::VERTICAL) {

  mList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::HORIZONTAL);

  mExpandButton = std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "+ " + header, [=]() {
    mExpanded = !mExpanded;
    if (mExpanded) {
      mExpandButton->setText("- " + header);
    } else {
      mExpandButton->setText("+ " + header);
    }
  }, 36, font);

  mList->addGuiComponent(mExpandButton);
  mGuiComponentList.emplace_back(mList);

}

void GuiExpandableList::render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) {
  calculateSize();
  calculatePosition(xPosition, yPosition, width, height);
  int xOffset = 0;
  int yOffset = 0;
  //mBoundingBox.setPosition((float)mXPosition + padding, (float)mYPosition);
  //mBoundingBox.setSize(sf::Vector2f((float)mWidth, (float)mHeight));
  //window->draw(mBoundingBox);
  for (const auto &component : mGuiComponentList) {
    int allowedWith = 0;
    int allowedHeight = 0;

    allowedWith = mWidth;
    allowedHeight = component->getHeight();

    component->render(window, mXPosition + xOffset, mYPosition + yOffset, allowedWith, allowedHeight);

    yOffset += component->getHeight();
    yOffset += padding;

    if (!mExpanded) {
      break;
    }
  }
}

void GuiExpandableList::clear() {
  auto header = mGuiComponentList.front();
  mGuiComponentList.clear();
  mGuiComponentList.emplace_back(header);
}

bool GuiExpandableList::checkMouse(sf::Vector2f mousePosition) {
  for (const auto &component : mGuiComponentList) {
    bool pressed = component->checkMouse(mousePosition);
    if (pressed) {
      return true;
    }
    if (!mExpanded) {
      break;
    }
  }
  return false;
}

void GuiExpandableList::calculateSize() {
  mWidth = 0;
  mHeight = 0;
  for (const auto &component : mGuiComponentList) {
    mHeight += component->getHeight();
    mHeight += padding;

    if (component->getWidth() > mWidth) {
      mWidth = component->getWidth();
    }

    if (!mExpanded) {
      break;
    }
  }

  mHeight -= padding;
}

void GuiExpandableList::setExpanded(bool expanded) {
  mExpanded = expanded;
}

bool GuiExpandableList::isExpanded() {
  return mExpanded;
}
