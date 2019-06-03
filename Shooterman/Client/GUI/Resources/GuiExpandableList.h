#pragma once

#include "GuiList.h"
#include "GuiButton.h"

class GuiExpandableList : public GuiList {
public:
  GuiExpandableList(GuiComponentPosition guiComponentPosition, std::string header, FONT font = MORRIS);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  void clear() override;

  bool checkMouse(sf::Vector2f mousePosition);

  void setExpanded(bool expanded);
  bool isExpanded();

private:
  bool mExpanded = false;
  std::shared_ptr<GuiButton> mExpandButton;
  std::shared_ptr<GuiText> mHeader;
  std::shared_ptr<GuiList> mList;

  void calculateSize();
};