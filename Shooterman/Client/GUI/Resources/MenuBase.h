#pragma once

#include <list>

#include <SFML/Graphics.hpp>

#include "../Resources/GUIComponentBuilder.h"
#include "../../MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/GameStateMessage.h"
#include "../../../Common/Trace.h"

#define FONT_FILE_PATH "Client/Resources/Fonts/"

class MenuBase : public Trace {
public:
  virtual void init();
  virtual void uninit();
  bool checkMouse(sf::Vector2f mousePosition);
  virtual bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition);
  virtual void handleNewText(sf::Uint32 newChar);
  
protected:
  std::list<std::shared_ptr<GUIComponent>> mComponentList;
};