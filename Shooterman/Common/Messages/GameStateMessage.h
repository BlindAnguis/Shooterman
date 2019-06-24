#pragma once

#include "Message.h"

enum GAME_STATE { NO_STATE, MAIN_MENU, DEBUG, LOBBY, CLIENT_LOBBY, PLAYING, SETUP_GAME, GAME_OVER, JOIN, EXIT, OPTIONS, PAUSE, MAP_EDITOR, PREVIOUS };

class GameStateMessage : Message {
public:
	GameStateMessage();
  GameStateMessage(sf::Packet packet);
  GameStateMessage(GAME_STATE newGameState);

  sf::Packet pack();

  void unpack(sf::Packet packet);

  GAME_STATE getGameState();
  std::string getGameStateAsString();

private:
  GAME_STATE mGameState;
};