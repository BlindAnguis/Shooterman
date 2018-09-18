#include "MainMenu.h"

MainMenu::MainMenu() {
  mName = "GUI";
  mDebugEnabled = true;

  loadFont("RobbieRocketpants.ttf");

  sf::Color textColor = sf::Color::Black;
  sf::Color textHighlightColor = sf::Color(0, 120, 0);

  mButtonList.push_back(new GUIButton("Shooterman", mFont, textColor, textColor, 72, 250, 36));

  mButtonList.push_back(new GUIButton("Host Game", mFont, textColor, textHighlightColor, 56, 250, 200, &changeGameStateToLobby));
  mButtonList.push_back(new GUIButton("Join Game", mFont, textColor, textHighlightColor, 56, 250, 260, &changeGameStateToJoin));
  mButtonList.push_back(new GUIButton("Options", mFont, textColor, textHighlightColor, 56, 250, 320));
  mButtonList.push_back(new GUIButton("Exit Game", mFont, textColor, textHighlightColor, 56, 250, 380, &exit));
}

MainMenu::~MainMenu() {
  for (GUIButton* button : mButtonList) {
    delete button;
  }
}

void MainMenu::changeGameStateToLobby() {
  GameStateMessage gsm(GAME_STATE::LOBBY);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}

void MainMenu::changeGameStateToJoin() {
  GameStateMessage gsm(GAME_STATE::JOIN);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}

void MainMenu::exit() {
  sf::Packet shutdownMessage;
  shutdownMessage << SHUT_DOWN;
  MessageHandler::get().pushSystemMessage(shutdownMessage);
}