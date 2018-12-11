#include "JoinMenu.h"

JoinMenu::JoinMenu() {
  mName = "CLIENT: JOIN_MENU";

  loadFont("RobbieRocketpants.ttf");

  sf::Color textColor = sf::Color::Black;
  sf::Color textHighlightColor = sf::Color(0, 120, 0);

  mButtonList.push_back(std::make_shared<GUIButton>("Join host", mFont, textColor, textColor, 72, 250, 36));

  mButtonList.push_back(std::make_shared<GUIButton>("Join 10.41.4.19", mFont, textColor, textHighlightColor, 56, 250, 200));
  mButtonList.push_back(std::make_shared<GUIButton>("Back", mFont, textColor, textHighlightColor, 56, 250, 380, &changeGameStateToMainMenu));
}

JoinMenu::~JoinMenu() { }

void JoinMenu::changeGameStateToMainMenu() {
  GameStateMessage gsm(GAME_STATE::MAIN_MENU);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}