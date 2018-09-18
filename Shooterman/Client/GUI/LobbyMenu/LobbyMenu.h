#pragma once

#include <list>

#include <SFML/Graphics.hpp>

#include "../Resources/MenuBase.h"

class LobbyMenu : public MenuBase {
public:
  LobbyMenu();
  ~LobbyMenu();
  
  static void changeGameStateToMainMenu();
  static void changeGameStateToPlaying();
};