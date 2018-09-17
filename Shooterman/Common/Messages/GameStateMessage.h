#pragma once

#include "Message.h"
#include "../MessageId.h"

enum GAME_STATE { MAIN_MENU, LOBBY, PLAYING, SETUP_GAME, GAME_OVER, JOIN, EXIT };

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