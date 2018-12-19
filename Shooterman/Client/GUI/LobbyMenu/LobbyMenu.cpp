#include "LobbyMenu.h"

LobbyMenu::LobbyMenu() {
  mName = "CLIENT: LOBBY_MENU";
  mDebugEnabled = true;

  mComponentList.push_back(GUIComponentBuilder::createTitle("Lobby", 250, 36));
  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Start Game", 250, 200, GAME_STATE::PLAYING));
  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Back", 250, 380, GAME_STATE::MAIN_MENU));
}

LobbyMenu::~LobbyMenu() { }