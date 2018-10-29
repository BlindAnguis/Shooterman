#include "MainMenu.h"

MainMenu::MainMenu() {
  mName = "MAIN_MENU";
  mDebugEnabled = true;

  loadFont("RobbieRocketpants.ttf");

  sf::Color textColor = sf::Color::Black;
  sf::Color textHighlightColor = sf::Color(0, 120, 0);

  mButtonList.push_back(std::make_shared<GUIButton>("Shooterman", mFont, textColor, textColor, 72, 250, 36));

  mButtonList.push_back(std::make_shared<GUIButton>("Host Game", mFont, textColor, textHighlightColor, 56, 250, 200, &changeGameStateToLobby));
  mButtonList.push_back(std::make_shared<GUIButton>("Join Game", mFont, textColor, textHighlightColor, 56, 250, 260, &changeGameStateToJoin));
  mButtonList.push_back(std::make_shared<GUIButton>("Options", mFont, textColor, textHighlightColor, 56, 250, 320, &changeGameStateToOptions));
  mButtonList.push_back(std::make_shared<GUIButton>("Exit Game", mFont, textColor, textHighlightColor, 56, 250, 380, &exit));
}

MainMenu::~MainMenu() { }

void MainMenu::changeGameStateToLobby() {
  GameStateMessage gsm(GAME_STATE::LOBBY);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}

void MainMenu::changeGameStateToJoin() {
  GameStateMessage gsm(GAME_STATE::JOIN);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}

void MainMenu::changeGameStateToOptions() {
	GameStateMessage gsm(GAME_STATE::OPTIONS);
	MessageHandler::get().pushGameStateMessage(gsm.pack());
}

void MainMenu::exit() {
  sf::Packet shutdownMessage;
  shutdownMessage << SHUT_DOWN;
  MessageHandler::get().pushSystemMessage(shutdownMessage);
}