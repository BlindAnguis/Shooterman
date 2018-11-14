#include "InputSystem.h"
#include <stdio.h>

InputSystem::InputSystem() {
  std::cout << "[SERVER: INPUT_SYSTEM] Subscribing to inputMessages for: " << mInputSubscriber.getId() << " : " << &mInputSubscriber << std::endl;
  MessageHandler::get().subscribeToInputMessages(&mInputSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mGameStateSubscriber);
  mCurrentGameState = GAME_STATE::LOBBY;
  mName = "Server: INPUT_SYSTEM";
}

InputSystem::~InputSystem() {
  MessageHandler::get().unsubscribeAll(&mInputSubscriber);
  MessageHandler::get().unsubscribeAll(&mGameStateSubscriber);
}

int InputSystem::getLatestInput() {
  int input = 0;
  int messageId;
  std::queue<sf::Packet> inputMessagesQueue = mInputSubscriber.getMessageQueue();
  sf::Packet inputMessage;

  if (!inputMessagesQueue.empty()) {
    inputMessage = inputMessagesQueue.front();
    inputMessagesQueue.pop();

    inputMessage >> messageId;
    //std::cout << "[SERVER_INPUT_SYSTEM] Message id: " << messageId << std::endl;

    inputMessage >> input;
    //std::cout << "[SERVER_INPUT_SYSTEM] input: " << input << std::endl;
  }

  return input;
}

GAME_STATE InputSystem::getLatestGameStateMessage() {
  GameStateMessage gsm;
  std::queue<sf::Packet> gameStateMessagesQueue = mGameStateSubscriber.getMessageQueue();

  if (!gameStateMessagesQueue.empty()) {
    gsm.unpack(gameStateMessagesQueue.front());
    gameStateMessagesQueue.pop();
  }

  GAME_STATE gameState = gsm.getGameState();
  if (gameState != GAME_STATE::NO_STATE) {
    TRACE_INFO("Changing current game state from: " <<mCurrentGameState << " to: " << gameState);
    mCurrentGameState = gameState;
  }
  return mCurrentGameState;
}

void InputSystem::handleInput() {
  int input = 0;
  int messageId;
  std::queue<sf::Packet> inputMessagesQueue = mInputSubscriber.getMessageQueue();
  sf::Packet inputMessage;

  if (!inputMessagesQueue.empty()) {
    inputMessage = inputMessagesQueue.front();
    inputMessagesQueue.pop();

    inputMessage >> messageId;
    //std::cout << "[SERVER_INPUT_SYSTEM] Message id: " << messageId << std::endl;

    inputMessage >> input;
    //std::cout << "[SERVER_INPUT_SYSTEM] input: " << input << std::endl;
    if (input > 0) {
      notify(input, 1);
    }
  }
  /*
  int input = getLatestInput();
  if (input > 0) {
    notify(input);
  }
  */
}

