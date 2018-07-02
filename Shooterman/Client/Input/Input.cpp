#include "Input.h"

#include <cstdint>

#include "../../Common/Trace.h"

Input::Input()
{
  mName = "INPUT";
  TRACE_INFO("Starting module...");
  mInputThread = new std::thread(&Input::readInput, this);
  TRACE_INFO("Starting module done");
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

    if (keyboardBitmask != 0) {
      sf::Packet inputKeyPacket;
      inputKeyPacket << INPUT_KEYS << keyboardBitmask;
      MessageHandler::get().pushInputMessage(inputKeyPacket);
    }
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
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
      TRACE_INFO("Preparing to shut down");
    } else {
      TRACE_WARNING("Unknown system message : " << messageId);
    }
  }
}

void Input::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mInputThread->join();
  delete mInputThread;
  TRACE_INFO("Shutdown of module done");
}