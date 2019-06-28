#pragma once

#include <list>
#include <memory>

#include "GuiComponent.h"

class GuiFrameComponent : public GuiComponent {
public:
  GuiFrameComponent(GuiComponentPosition guiComponentPosition, sf::RectangleShape frameSize);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;

  bool checkMouse(sf::Vector2f mousePosition);

  void addGuiComponent(std::shared_ptr<GuiComponent> guiComponent);
  void removeGuiComponent(std::shared_ptr<GuiComponent> guiComponent);

  int getNumberOfComponents();
  std::shared_ptr<GuiComponent> getComponent(int index);
  bool isMouseClickInsideFrame(sf::Vector2f mousePosition);

  virtual void clear();

protected:
  int padding = 20;
  std::list<std::shared_ptr<GuiComponent>> mGuiComponentList;


private:
  void calculateSize();
  sf::RectangleShape mFrameSize;
};