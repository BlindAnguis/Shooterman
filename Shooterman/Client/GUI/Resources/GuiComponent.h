#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

enum GuiComponentPosition { CENTER, TOP, BOTTOM, LEFT, RIGHT, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

class GuiComponent {
public:
  GuiComponent();
  explicit GuiComponent(GuiComponentPosition guiComponentPosition);

  virtual void render(std::shared_ptr<sf::RenderWindow>, int xPosition, int yPosition, int width, int height) = 0;
  virtual bool checkMouse(sf::Vector2f mousePosition);

  int getXPosition();
  int getYPosition();
  int getWidth();
  int getHeight();

protected:
  GuiComponentPosition mGuiComponentPosition;
  int mXPosition = 0;
  int mYPosition = 0;
  int mWidth = 0;
  int mHeight = 0;
  sf::RectangleShape mBoundingBox;

  void calculatePosition(int xPosition, int yPosition, int width, int height);
};