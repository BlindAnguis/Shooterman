#include "MainMenu.h"

MainMenu::MainMenu() {
  mName = "CLIENT: MAIN_MENU";
  mDebugEnabled = true;

  mComponentList.push_back(GUIComponentBuilder::createTitle("Shooterman", 250, 36));
  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Host Game", 250, 200, GAME_STATE::LOBBY));
  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Join Game", 250, 260, GAME_STATE::JOIN));
  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Options", 250, 320, GAME_STATE::OPTIONS));
  mComponentList.push_back(GUIComponentBuilder::createCustomActionButton("Exit Game", 250, 380, []() {
    sf::Packet shutdownMessage;
    shutdownMessage << SHUT_DOWN;
    MessageHandler::get().pushSystemMessage(shutdownMessage);
  }));
}

MainMenu::~MainMenu() { }