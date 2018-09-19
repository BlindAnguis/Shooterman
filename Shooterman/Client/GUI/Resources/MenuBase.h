#pragma once

#include <list>

#include <SFML/Graphics.hpp>

#include "../Resources/GUIButton.h"
#include "../../MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/GameStateMessage.h"
#include "../../../Common/Trace.h"

#define FONT_FILE_PATH "Client/Resources/Fonts/"

class MenuBase : public Trace {
public:
  bool checkMouse(sf::Vector2f mousePosition);
  void render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition);
  
protected:
  sf::Font mFont;
  std::list<std::shared_ptr<GUIButton>> mButtonList;

  void loadFont(std::string fontName);
};