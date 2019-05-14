#include "InputSystem.h"
#include <stdio.h>

InputSystem::InputSystem()
  :
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get())
{
  //std::cout << "[SERVER: INPUT_SYSTEM] Subscribing to inputMessages for: " << mInputSubscriber.getId() << " : " << &mInputSubscriber << std::endl;
 
  MessageHandler::get().subscribeTo("GameState", &mGameStateSubscriber);
  mCurrentGameState = GAME_STATE::LOBBY;
  mName = "Server: INPUT_SYSTEM";
  mAttack = [](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {};
}

InputSystem::~InputSystem() {
  MessageHandler::get().unsubscribeTo("ServerInputList", &mInputSubscriber);
  MessageHandler::get().unsubscribeTo("GameState", &mGameStateSubscriber);
}

InputSystem& InputSystem::get() {
  static InputSystem instance;
  return instance;
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
    //TRACE_INFO("Changing current game state from: " << mCurrentGameState << " to: " << gameState);
    mCurrentGameState = gameState;
  }
  return mCurrentGameState;
}

void InputSystem::handleInput() {
  //TRACE_INFO("Handle input");
  std::queue<sf::Packet> inputMessagesQueue = getInput();
  while (!inputMessagesQueue.empty()) {
    InputMessage im(inputMessagesQueue.front());
    inputMessagesQueue.pop();
    //TRACE_INFO("New input from queue");
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

    if (im.getKeyboardBitMask() & LEFT_MOUSE) {
      //TRACE_INFO("Started a new attack!");
      mAttack(playerEntity->id, im.getKeyboardBitMask(), im.getMousePosition());
    }

    if (im.getId() > 0) {
      notify(im);
    }
  }
}
