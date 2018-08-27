#pragma once

#include "Message.h"
#include "../MessageId.h"

enum GAME_STATE { STATE_MAIN_MENU, STATE_LOBBY, STATE_PLAYING };

class GameStateMessage : Message {
public:
  GameStateMessage() : mGameState(GAME_STATE::STATE_MAIN_MENU) { }
  GameStateMessage(GAME_STATE newGameState) : mGameState(newGameState) { }

  sf::Packet pack() {
    sf::Packet packet;
    packet << CHANGE_GAME_STATE;
    packet << mGameState;
    return packet;
  }

  void unpack(sf::Packet packet) {
    int gameState;
    packet >> gameState;
    mGameState = (GAME_STATE)gameState;
  }

  GAME_STATE getGameState() { return mGameState; }

private:
  GAME_STATE mGameState;
};