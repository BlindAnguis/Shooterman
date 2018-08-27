#pragma once

#include "Message.h"
#include "../MessageId.h"

enum GAME_STATE { STATE_MAIN_MENU, STATE_LOBBY, STATE_PLAYING };

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