#pragma once

#include <list>

#include <SFML/Graphics.hpp>

#include "GuiFrame.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/GameStateMessage.h"
#include "../../../Common/Trace.h"

#define FONT_FILE_PATH "Client/Resources/Fonts/"

class MenuBase : public Trace {
public:
  virtual ~MenuBase();
  virtual void init();
  virtual void uninit();
  virtual bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition);
  virtual bool checkMouse(sf::Vector2f mousePosition);
  virtual void handleNewText(sf::Uint32 newChar);
  
protected:
  std::shared_ptr<Frame> mGuiFrame;
};