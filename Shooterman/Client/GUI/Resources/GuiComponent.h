#pragma once

#include <functional>
#include <string>

#include <SFML/Graphics.hpp>

#include "../../../Common/Trace.h"

class GUIComponent : public Trace {
public:
  virtual void render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) = 0;
  virtual bool isPressed(sf::Vector2f mousePosition) { return false; };
  virtual void setText(std::string newText) {};
  virtual std::string getText() { return ""; };
  virtual void setRenderBounds(bool renderBounds) {};
  virtual void setPosition(float xPosition, float yPosition) {}
  virtual int getWidth() { return 0; };
  virtual int getHeight() { return 0; };
  virtual void setMaxValue(int value) {}
  virtual void setCurrentValue(int value) {}
};