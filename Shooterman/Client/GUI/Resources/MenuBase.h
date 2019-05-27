#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "GuiFrame.h"
#include "../../../Common/Trace.h"

class MenuBase : public Trace {
public:
  virtual ~MenuBase();
  virtual void init();
  virtual void uninit();
  virtual void reset();
  virtual bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition);
  virtual bool checkMouse(sf::Vector2f mousePosition);
  virtual void handleNewText(sf::Uint32 newChar);
  
protected:
  std::shared_ptr<Frame> mGuiFrame;
};