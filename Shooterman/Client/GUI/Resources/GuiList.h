#pragma once

#include <list>
#include <memory>

#include "GuiComponent.h"

enum GuiListDirection { VERTICAL, HORIZONTAL };

class GuiList : public GuiComponent {
public:
  explicit GuiList(GuiComponentPosition guiComponentPosition, GuiListDirection guiListDirection);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  
  bool checkMouse(sf::Vector2f mousePosition);

  void addGuiComponent(std::shared_ptr<GuiComponent> guiComponent);
  void removeGuiComponent(std::shared_ptr<GuiComponent> guiComponent);

  int getNumberOfComponents();
  std::shared_ptr<GuiComponent> getComponent(int index);

  virtual void clear();

protected:
  int padding = 20;
  std::list<std::shared_ptr<GuiComponent>> mGuiComponentList;


private:
  GuiListDirection mGuiListDirection;

  void calculateSize();
};