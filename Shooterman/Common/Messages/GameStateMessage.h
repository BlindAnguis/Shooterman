#pragma once

#include "Message.h"

enum GAME_STATE { NO_STATE, MAIN_MENU, LOBBY, CLIENT_LOBBY, PLAYING, SETUP_GAME, GAME_OVER, JOIN, EXIT, OPTIONS, PAUSE, PREVIOUS };

class GameStateMessage : Message {
public:
	GameStateMessage();
  GameStateMessage(GAME_STATE newGameState);

  sf::Packet pack();

  void unpack(sf::Packet packet);

  GAME_STATE getGameState();

private:
  GAME_STATE mGameState;
};