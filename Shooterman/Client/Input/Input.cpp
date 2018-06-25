#include "Input.h"

#include <cstdint>

#include <iostream>

Input::Input()
{
  std::cout << "[INPUT] Starting module..." << std::endl;
  mInputThread = new std::thread(&Input::readInput, this);
  std::cout << "[INPUT] Starting module done" << std::endl;
}

void Input::readInput() {
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);

  std::uint32_t keyboardBitmask;
  while (mRunning) {
    keyboardBitmask = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      keyboardBitmask += A_KEY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      keyboardBitmask += D_KEY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      keyboardBitmask += W_KEY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      keyboardBitmask += S_KEY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
      sf::Packet shutDownMessage;
      shutDownMessage << SHUT_DOWN;
      MessageHandler::get().pushSystemMessage(shutDownMessage);
    }
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
	//	sf::Packet shutDownMessage;
	//	shutDownMessage << SHUT_DOWN;
	//	MessageHandler::get().pushSystemMessage(shutDownMessage);
	//}

    if (keyboardBitmask != 0) {
      sf::Packet inputKeyPacket;
      inputKeyPacket << INPUT_KEYS << keyboardBitmask;
      MessageHandler::get().pushInputMessage(inputKeyPacket);
    }
    sf::sleep(sf::milliseconds(20));
    handleSystemMessages();
  }
}

void Input::handleSystemMessages() {
  std::queue<sf::Packet> systemMessageQueue = mSystemMessageSubscriber.getMessageQueue();
  sf::Packet systemMessage;
  while (!systemMessageQueue.empty()) {
    systemMessage = systemMessageQueue.front();
    systemMessageQueue.pop();

    int messageId = 0;
    systemMessage >> messageId;
    if (messageId == SHUT_DOWN) {
      mRunning = false;
      std::cout << "[INPUT] Preparing to shut down" << std::endl;
    } else {
      std::cout << "[INPUT] Unknown system message: " << messageId << std::endl;
    }
  }
}

void Input::shutDown() {
  std::cout << "[INPUT] Shutdown of module requested..." << std::endl;
  mRunning = false;
  mInputThread->join();
  delete mInputThread;
  std::cout << "[INPUT] Shutdown of module done" << std::endl;
}