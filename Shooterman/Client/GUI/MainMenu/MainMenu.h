#pragma once

#include <list>

#include <SFML/Graphics.hpp>

#include "../Resources/MenuBase.h"
#include "../../../Common/Trace.h"

class MainMenu : public MenuBase {
public:
  MainMenu();
  ~MainMenu();

  // Button callbacks
  static void changeGameStateToLobby();
  static void changeGameStateToJoin();
  static void exit();
};