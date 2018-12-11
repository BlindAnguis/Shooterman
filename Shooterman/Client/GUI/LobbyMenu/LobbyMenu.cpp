#include "LobbyMenu.h"

LobbyMenu::LobbyMenu() {
  mName = "CLIENT: LOBBY_MENU";
  mDebugEnabled = true;

  loadFont("RobbieRocketpants.ttf");

  sf::Color textColor = sf::Color::Black;
  sf::Color textHighlightColor = sf::Color(0, 120, 0);

  mButtonList.push_back(std::make_shared<GUIButton>("Lobby", mFont, textColor, textColor, 72, 250, 36));

  mButtonList.push_back(std::make_shared<GUIButton>("Start Game", mFont, textColor, textHighlightColor, 56, 250, 200, &changeGameStateToPlaying));
  mButtonList.push_back(std::make_shared<GUIButton>("Back", mFont, textColor, textHighlightColor, 56, 250, 380, &changeGameStateToMainMenu));
}

LobbyMenu::~LobbyMenu() { }

void LobbyMenu::changeGameStateToPlaying() {
  GameStateMessage gsm(GAME_STATE::PLAYING);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}

void LobbyMenu::changeGameStateToMainMenu() {
  GameStateMessage gsm(GAME_STATE::MAIN_MENU);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}