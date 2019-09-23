#include "GameStateMessage.h"

#include "../MessageId.h"

GameStateMessage::GameStateMessage() :
  mGameState(GAME_STATE::NO_STATE) { }

GameStateMessage::GameStateMessage(sf::Packet packet) {
  unpack(packet);
}

GameStateMessage::GameStateMessage(GAME_STATE newGameState) :
  mGameState(newGameState) { }

sf::Packet GameStateMessage::pack() {
  sf::Packet packet;
  packet << MessageId::CHANGE_GAME_STATE;
  packet << mGameState;
  return packet;
}

void  GameStateMessage::unpack(sf::Packet packet) {
  int gameState;
  packet >> gameState;
  mGameState = (GAME_STATE)gameState;
}

GAME_STATE GameStateMessage::getGameState() {
  return mGameState; 
}

std::string GameStateMessage::getGameStateAsString() {
  switch (mGameState) {
  case NO_STATE:
    return "NO_STATE";
  case MAIN_MENU:
    return "MAIN_MENU";
  case DEBUG:
    return "DEBUG";
  case LOBBY:
    return "LOBBY";
  case CLIENT_LOBBY:
    return "CLIENT_LOBBY";
  case PLAYING:
    return "PLAYING";
  case SETUP_GAME:
    return "SETUP_GAME";
  case GAME_OVER:
    return "GAME_OVER";
  case JOIN:
    return "JOIN";
  case EXIT:
    return "EXIT";
  case OPTIONS:
    return "OPTIONS";
  case PAUSE:
    return "PAUSE";
  case MAP_EDITOR:
    return "MAP_EDITOR";
  case SCORE_BOARD:
    return "SCORE_BOARD";
  case PREVIOUS:
    return "PREVIOUS";
  default:
    return std::to_string(mGameState) + " not added to GameStateMessage.cpp";
    break;
  }
}