#include "InputSystem.h"
#include <stdio.h>

InputSystem::InputSystem() {
  std::cout << "[SERVER: INPUT_SYSTEM] Subscribing to inputMessages for: " << mInputSubscriber.getId() << " : " << &mInputSubscriber << std::endl;
  MessageHandler::get().subscribeToInputMessages(&mInputSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mGameStateSubscriber);
  mCurrentGameState = GAME_STATE::LOBBY;
  mName = "Server: INPUT_SYSTEM";
  mAttack = [](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {};
}

InputSystem::~InputSystem() {
  MessageHandler::get().unsubscribeAll(&mInputSubscriber);
  MessageHandler::get().unsubscribeAll(&mGameStateSubscriber);
}

InputMessage InputSystem::getLatestInput() {
  int input = 0;
  int messageId;
  std::queue<sf::Packet> inputMessagesQueue = mInputSubscriber.getMessageQueue();
  sf::Packet inputMessage;
  InputMessage im;
  if (!inputMessagesQueue.empty()) {
    inputMessage = inputMessagesQueue.front();
    inputMessagesQueue.pop();

    inputMessage >> messageId;
    //std::cout << "[SERVER_INPUT_SYSTEM] Message id: " << messageId << std::endl;

    im.unpack(inputMessage);

    inputMessage >> input;
    //std::cout << "[SERVER_INPUT_SYSTEM] input: " << input << std::endl;
  }

  return im;
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
  std::queue<sf::Packet> inputMessagesQueue = mInputSubscriber.getMessageQueue();

  while (!inputMessagesQueue.empty()) {
    InputMessage im(inputMessagesQueue.front());
    inputMessagesQueue.pop();

    //std::cout << "[SERVER_INPUT_SYSTEM] Message id: " << messageId << std::endl;
    //std::cout << "[SERVER_INPUT_SYSTEM] input: " << input << std::endl;

    if (im.getKeyboardBitMask() & LEFT_MOUSE) {
      //TRACE_INFO("Pressed Attack!");
      mAttack(mPlayersMap->at(im.getId()).second->id, im.getKeyboardBitMask(), im.getMousePosition());
    }

    if (im.getId() > 0) {
      notify(im, -1);
    }
  }
}

