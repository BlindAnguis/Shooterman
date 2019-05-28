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
  virtual int getWidth();
  virtual int getHeight();

  int getId();

  bool operator == (const GuiComponent& guiComponent) const { return mId == guiComponent.mId; }
  bool operator != (const GuiComponent& guiComponent) const { return !operator==(guiComponent); }

protected:
  int mId;
  GuiComponentPosition mGuiComponentPosition;
  int mXPosition = 0;
  int mYPosition = 0;
  int mWidth = 0;
  int mHeight = 0;
  int mPadding = 20;
  sf::RectangleShape mBoundingBox;

  void calculatePosition(int xPosition, int yPosition, int width, int height);
};