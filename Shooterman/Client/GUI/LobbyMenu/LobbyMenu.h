#pragma once

#include <list>

#include <SFML/Graphics.hpp>

#include "../Resources/GUIButton.h"
#include "../../../Common/Trace.h"

#define FONT_FILE_PATH "Client/Resources/Fonts/"

class LobbyMenu : Trace {
public:
  LobbyMenu();
  ~LobbyMenu();

  BUTTON_PRESSED checkMouse(sf::Vector2f mousePosition);
  void render(sf::RenderWindow* window);

private:
  sf::Font mFont;
  std::list<GUIButton*> mButtonList;
};