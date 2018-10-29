#include "OptionsMenu.h"

#include <SFML/Graphics.hpp>

OptionsMenu::OptionsMenu()
{
  mName = "OPTIONS_MENU";
  mDebugEnabled = true;

  loadFont("RobbieRocketpants.ttf");

  sf::Color textColor = sf::Color::Black;
  sf::Color textHighlightColor = sf::Color(0, 120, 0);

  mButtonList.push_back(std::make_shared<GUIButton>("Options", mFont, textColor, textColor, 72, 250, 36));
  //mButtonList.push_back(std::make_shared<GUIButton>("Start Game", mFont, textColor, textHighlightColor, 56, 250, 200, &changeGameStateToPlaying));
  mButtonList.push_back(std::make_shared<GUIButton>("Back", mFont, textColor, textHighlightColor, 56, 250, 380, &changeGameStateToMainMenu));
}


OptionsMenu::~OptionsMenu()
{
}


void OptionsMenu::changeGameStateToMainMenu() {
  GameStateMessage gsm(GAME_STATE::MAIN_MENU);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
}