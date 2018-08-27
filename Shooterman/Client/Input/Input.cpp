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
  mCurrentGameState = GAME_STATE::STATE_MAIN_MENU;

  std::uint32_t keyboardBitmask;
  while (mRunning) {
    switch (mCurrentGameState) {
    case GAME_STATE::STATE_MAIN_MENU:
      // Do nothing
      break;
    case GAME_STATE::STATE_LOBBY:
      // Do nothing
      break;
    case GAME_STATE::STATE_PLAYING:
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

      if (keyboardBitmask != 0) {
        sf::Packet inputKeyPacket;
        inputKeyPacket << INPUT_KEYS << keyboardBitmask;
        MessageHandler::get().pushInputMessage(inputKeyPacket);
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        GameStateMessage gsm(GAME_STATE::STATE_MAIN_MENU);
        MessageHandler::get().pushSystemMessage(gsm.pack());
      }

      break;
    default:
      TRACE_ERROR("Unknown game state: " << mCurrentGameState);
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
    switch (messageId) {
    case SHUT_DOWN:
      mRunning = false;
      TRACE_INFO("Preparing to shut down");
      break;
    case CHANGE_GAME_STATE: {
      GameStateMessage gsm;
      gsm.unpack(systemMessage);
      mCurrentGameState = gsm.getGameState();
      break; }
    default:
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