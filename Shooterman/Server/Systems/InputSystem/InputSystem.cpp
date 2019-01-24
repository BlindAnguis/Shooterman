#include "InputSystem.h"
#include <stdio.h>

InputSystem::InputSystem() {}

InputSystem::InputSystem(ComponentManager<HealthComponent>* healthComponentManager) : mHealthComponentManager(healthComponentManager) {
  //std::cout << "[SERVER: INPUT_SYSTEM] Subscribing to inputMessages for: " << mInputSubscriber.getId() << " : " << &mInputSubscriber << std::endl;
  MessageHandler::get().subscribeToGameStateMessages(&mGameStateSubscriber);
  mCurrentGameState = GAME_STATE::LOBBY;
  mName = "Server: INPUT_SYSTEM";
  mAttack = [](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {};
}

InputSystem::~InputSystem() {
  MessageHandler::get().unsubscribeTo("ServerInputList", &mInputSubscriber);
  MessageHandler::get().unsubscribeAll(&mGameStateSubscriber);
}

std::queue<sf::Packet> InputSystem::getInput() {
  if (!mIsSubscribedToInput) {
    mIsSubscribedToInput = MessageHandler::get().subscribeTo("ServerInputList", &mInputSubscriber);
  }
  return mInputSubscriber.getMessageQueue();
}

InputMessage InputSystem::getLatestInput() {
  int input = 0;
  int messageId;
  std::queue<sf::Packet> inputMessagesQueue = getInput();
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
    TRACE_INFO("Changing current game state from: " << mCurrentGameState << " to: " << gameState);
    mCurrentGameState = gameState;
  }
  return mCurrentGameState;
}

void InputSystem::handleInput() {
  std::queue<sf::Packet> inputMessagesQueue = getInput();

  while (!inputMessagesQueue.empty()) {
    InputMessage im(inputMessagesQueue.front());
    inputMessagesQueue.pop();

    //std::cout << "[SERVER_INPUT_SYSTEM] Message id: " << messageId << std::endl;
    //std::cout << "[SERVER_INPUT_SYSTEM] input: " << input << std::endl;

    Entity* playerEntity = mPlayersMap->at(im.getId())->getEntity();

    // Player is dead
    auto playerHealth = mHealthComponentManager->getComponent(playerEntity->id);
    if (playerHealth && !playerHealth->isAlive) {
      continue;
    }

    if (im.getKeyboardBitMask() & LEFT_MOUSE) {
      //TRACE_INFO("Pressed Attack!");
      mAttack(playerEntity->id, im.getKeyboardBitMask(), im.getMousePosition());
    }

    if (im.getId() > 0) {
      notify(im);
    }
  }
}
