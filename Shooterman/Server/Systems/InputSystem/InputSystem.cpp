#include "InputSystem.h"
#include <stdio.h>
#include "../../../Common/Interfaces.h"

InputSystem::InputSystem(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler) {
  mCurrentGameState = GAME_STATE::LOBBY;
  mName = "Server: INPUT_SYSTEM";
  mAttack = [](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {};
}

InputSystem::~InputSystem() {
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_INPUT_LIST, &mInputSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_GAME_STATE, &mGameStateSubscriber);
  TRACE_DEBUG1("Enter Destructor");
}

void InputSystem::resetSystem() {
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_INPUT_LIST, &mInputSubscriber);
  mIsSubscribedToInput = false;
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_GAME_STATE, &mGameStateSubscriber);
  mGameStateSubscriber.clearMessages();
  mInputSubscriber.clearMessages();
  mIsSubscribedToGameState = false;
  mCurrentGameState = GAME_STATE::LOBBY;
  mAttack = [](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {};
  mObservers.clear();
}

std::queue<sf::Packet> InputSystem::getInput() {
  if (!mIsSubscribedToInput) {
    mIsSubscribedToInput = mMessageHandler->subscribeTo(Interfaces::SERVER_INPUT_LIST, &mInputSubscriber);
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
  if (!mIsSubscribedToGameState) {
    mIsSubscribedToGameState = mMessageHandler->subscribeTo(Interfaces::SERVER_GAME_STATE, &mGameStateSubscriber);
  }

  std::queue<sf::Packet> gameStateMessagesQueue = mGameStateSubscriber.getMessageQueue();

  if (!gameStateMessagesQueue.empty()) {
    sf::Packet packet = gameStateMessagesQueue.front();
    gameStateMessagesQueue.pop();
    int id = -1;
    packet >> id;

    if (id == MessageId::CHANGE_GAME_STATE) {
      GameStateMessage gsm(packet);
      GAME_STATE gameState = gsm.getGameState();

      if (gameState != mCurrentGameState) {
        mGameStateSubscriber.reverseSendMessage(gsm.pack());
      }

      if (gameState != GAME_STATE::NO_STATE) {
        TRACE_DEBUG1("Changing current game state from: " << mCurrentGameState << " to: " << gameState);
        mCurrentGameState = gameState;
      }
    }
  }
  return mCurrentGameState;
}

void InputSystem::handleInput() {
  //TRACE_INFO("Handle input");
  std::queue<sf::Packet> inputMessagesQueue = getInput();
  while (!inputMessagesQueue.empty()) {
    InputMessage im(inputMessagesQueue.front());
    inputMessagesQueue.pop();
    //std::cout << "[SERVER_INPUT_SYSTEM] Message id: " << messageId << std::endl;
    //std::cout << "[SERVER_INPUT_SYSTEM] input: " << input << std::endl;

    Entity* playerEntity = mPlayersMap->at(im.getId())->getEntity();

    // Ignore input while player is dead or attacking
    auto playerHealth = ComponentManager<HealthComponent>::get().getComponent(playerEntity->id);
    auto player = ComponentManager<PlayerComponent>::get().getComponent(playerEntity->id);

    if (playerHealth && !playerHealth->isAlive || player->state == PlayerState::Attacking) {
      //TRACE_INFO("Player is already attacking - Ignore input");
      continue;
    }

    if (im.getKeyboardBitMask() & LEFT_MOUSE_VALUE) {
      //TRACE_INFO("Started a new attack!");
      mAttack(playerEntity->id, im.getKeyboardBitMask(), im.getMousePosition());
    }

    if (im.getId() > 0) {
      notify(im);
    }
  }
}
