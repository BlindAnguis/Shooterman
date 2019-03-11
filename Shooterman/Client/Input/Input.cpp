#include "Input.h"

#include <cstdint>

#include "../../Common/Trace.h"

Input::Input() {
  mName = "CLIENT: INPUT";
  TRACE_INFO("Starting module...");
  mInputThread = std::make_unique<std::thread>(&Input::readInput, this);
  TRACE_INFO("Starting module done");
}
Input::~Input() {
  MessageHandler::get().unsubscribeTo("GameState", &mGameStateMessageSubscriber);
}

void Input::readInput() {
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  MessageHandler::get().subscribeTo("GameState",&mGameStateMessageSubscriber);
  bool subscribedToMouse = MessageHandler::get().subscribeTo("MousePosition", &mMouseMessageSubscriber);
  Interface* pc = new Interface();
  MessageHandler::get().publishInterface("ClientInputList", pc);
  mCurrentGameState = GAME_STATE::MAIN_MENU;
  while (!MessageHandler::get().subscribeTo("ClientDebugMenu", &mDebugSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  AddDebugButtonMessage debMess(mDebugSubscriber.getId(), "CLient input debug traces");
  mDebugSubscriber.reverseSendMessage(debMess.pack());
  std::uint32_t keyboardBitmask;
  while (mRunning) {
    switch (mCurrentGameState) {
      case GAME_STATE::MAIN_MENU:
        // Do nothing
        break;
      case GAME_STATE::LOBBY:
        // Do nothing
        break;
      case GAME_STATE::JOIN:
        // Do nothing
        break;
      case GAME_STATE::PAUSE:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && recentlyChangedState) {
          break;
        }
        recentlyChangedState = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
          GameStateMessage gsm(GAME_STATE::PLAYING);
          
		      MessageHandler::get().pushGameStateMessage("GameState", gsm.pack());
          recentlyChangedState = true;
        }
        break;
      case GAME_STATE::PLAYING:
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && recentlyChangedState) {
          break;
        }
        recentlyChangedState = false;
        keyboardBitmask = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
          keyboardBitmask |= A_KEY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          keyboardBitmask |= D_KEY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          keyboardBitmask |= W_KEY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          keyboardBitmask |= S_KEY;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          keyboardBitmask |= LEFT_MOUSE;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
          keyboardBitmask |= RIGHT_MOUSE;
        }

        getLatestMousePosition();
        InputMessage im(keyboardBitmask, mLastMousePosition.x, mLastMousePosition.y);
        pc->pushMessage(im.pack());

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
          GameStateMessage gsm(GAME_STATE::PAUSE);
          MessageHandler::get().pushGameStateMessage("GameState", gsm.pack());
          recentlyChangedState = true;
        }
        break;
      }
      case GAME_STATE::OPTIONS:
        break;
      default:
        TRACE_ERROR("Unknown game state: " << mCurrentGameState);
    }

    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
    handleGameStateMessages();
    handleSystemMessages();
    handleDebugMessages();
    if (!subscribedToMouse) {
      subscribedToMouse = MessageHandler::get().subscribeTo("MousePosition", &mMouseMessageSubscriber);
    }

  }

  MessageHandler::get().unpublishInterface("ClientInputList");
  delete pc;
  MessageHandler::get().unsubscribeAll(&mSystemMessageSubscriber);
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
    default:
      TRACE_WARNING("Unknown system message : " << messageId);
    }
  }
}

void Input::handleGameStateMessages() {
  std::queue<sf::Packet> gameStateMessageQueue = mGameStateMessageSubscriber.getMessageQueue();
  sf::Packet gameStateMessage;
  while (!gameStateMessageQueue.empty()) {
    gameStateMessage = gameStateMessageQueue.front();
    gameStateMessageQueue.pop();

    GameStateMessage gsm;
    gsm.unpack(gameStateMessage);
    mCurrentGameState = gsm.getGameState();
  }
}

void Input::handleDebugMessages() {
  std::queue<sf::Packet> debugMessageQueue = mDebugSubscriber.getMessageQueue();
  sf::Packet debugMessage;
  while (!debugMessageQueue.empty()) {
    debugMessage = debugMessageQueue.front();
    debugMessageQueue.pop();
    TRACE_DEBUG("Toggle debug traces");
    mDebugEnabled = !mDebugEnabled;
    TRACE_DEBUG("Toggle debug traces");
  }
}

void Input::getLatestMousePosition() {
  std::queue<sf::Packet> mouseMessageQueue = mMouseMessageSubscriber.getMessageQueue();
  sf::Packet mouseMessage;

  while (!mouseMessageQueue.empty()) {
    mouseMessage = mouseMessageQueue.front();
    mouseMessageQueue.pop();

    MouseMessage mm;
    mm.unpack(mouseMessage);
    if (mm.getPosition() != sf::Vector2i()) {
      mLastMousePosition = mm.getPosition();
    }
  }  
}

void Input::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mInputThread->join();
  TRACE_INFO("Shutdown of module done");
}