#include "InputSystem.h"
#include <stdio.h>

InputSystem::InputSystem() {
  std::cout << "[SERVER: INPUT_SYSTEM] Subscribing to inputMessages for: " << mInputSubscriber.getId() << " : " << &mInputSubscriber << std::endl;
  MessageHandler::get().subscribeToInputMessages(&mInputSubscriber);
}

InputSystem::~InputSystem() {
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
      notify(input);
    }
  }
  /*
  int input = getLatestInput();
  if (input > 0) {
    notify(input);
  }
  */
}

