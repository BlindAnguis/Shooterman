#include "OptionsMenu.h"

#include <SFML/Graphics.hpp>

OptionsMenu::OptionsMenu() {
  mName = "CLIENT: OPTIONS_MENU";
  mDebugEnabled = true;

  mComponentList.push_back(GUIComponentBuilder::createTitle("Options", 250, 36));
  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Back", 250, 380, GAME_STATE::MAIN_MENU));
}

OptionsMenu::~OptionsMenu() { }
