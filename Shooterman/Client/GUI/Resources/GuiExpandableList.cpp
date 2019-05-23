#include "GuiExpandableList.h"

#include "GuiComponentFactory.h"

GuiExpandableList::GuiExpandableList(GuiComponentPosition guiComponentPosition, std::string header, FONT font)
  : GuiList(guiComponentPosition, GuiListDirection::VERTICAL) {

  mList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::HORIZONTAL);

  mExpandButton = std::make_shared<GuiButton>(GuiComponentPosition::LEFT, "+", [this]() {
    mExpanded = !mExpanded;
    if (mExpanded) {
      mExpandButton->setText("-");
    } else {
      mExpandButton->setText("+");
    }
  }, 36, font);

  mHeader = std::make_shared<GuiText>(GuiComponentPosition::LEFT, header, 36, font);
  mList->addGuiComponent(mExpandButton);
  mList->addGuiComponent(mHeader);
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

bool GuiExpandableList::checkMouse(sf::Vector2f mousePosition) {
  for (const auto &component : mGuiComponentList) {
    component->checkMouse(mousePosition);
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
