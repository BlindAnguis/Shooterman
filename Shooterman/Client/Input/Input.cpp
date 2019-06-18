#include "Input.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../../Common/KeyBindings.h"
#include "../../Common/Constants.h"
#include "../../Common/Messages/MouseMessage.h"
#include "../../Common/Messages/InputMessage.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"

Input::Input() {
  mName = "CLIENT: INPUT";
  TRACE_INFO("Starting module...");
  mInputThread = std::make_unique<std::thread>(&Input::run, this);
  TRACE_INFO("Starting module done");
}
Input::~Input() {
  TRACE_FUNC_ENTER();
  MessageHandler::get().unsubscribeTo("ClientGameState", &mGameStateMessageSubscriber);
  TRACE_FUNC_EXIT();
}

void Input::run() {
  TRACE_FUNC_ENTER();
  MessageHandler::get().subscribeTo("ClientSystemMessage", &mSystemMessageSubscriber);
  MessageHandler::get().subscribeTo("ClientGameState", &mGameStateMessageSubscriber);
  bool subscribedToMouse = MessageHandler::get().subscribeTo("MousePosition", &mMouseMessageSubscriber);
  MessageHandler::get().publishInterface("ClientInputList", &mClientInputInterface);
  mCurrentGameState = GAME_STATE::MAIN_MENU;
  while (!setupDebugMessages("Client", "Input")) {
    sf::sleep(sf::milliseconds(5));
  }
  std::uint32_t keyboardBitmask;
  while (mRunning) {
    switch (mCurrentGameState) {
      case GAME_STATE::MAIN_MENU:
        // Do nothing
        break;
      case GAME_STATE::LOBBY:
      case GAME_STATE::CLIENT_LOBBY:
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
          TRACE_SEND("New Game State: PLAYING");
		      mGameStateMessageSubscriber.reverseSendMessage(gsm.pack());
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
        mClientInputInterface.pushMessage(im.pack());

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
          GameStateMessage gsm(GAME_STATE::PAUSE);
          TRACE_SEND("New Game State: PAUSE");
          mGameStateMessageSubscriber.reverseSendMessage(gsm.pack());
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

  teardownDebugMessages();
  MessageHandler::get().unpublishInterface("ClientInputList");
  MessageHandler::get().unsubscribeTo("ClientSystemMessage", &mSystemMessageSubscriber);
  TRACE_FUNC_EXIT();
}

void Input::handleSystemMessages() {
  TRACE_FUNC_ENTER();
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
  TRACE_FUNC_EXIT();
}

void Input::handleGameStateMessages() {
  TRACE_FUNC_ENTER();
  std::queue<sf::Packet> gameStateMessageQueue = mGameStateMessageSubscriber.getMessageQueue();
  sf::Packet gameStateMessage;
  while (!gameStateMessageQueue.empty()) {
    gameStateMessage = gameStateMessageQueue.front();
    gameStateMessageQueue.pop();

    int id = -1;
    gameStateMessage >> id;

    if (id == CHANGE_GAME_STATE) {
      GameStateMessage gsm(gameStateMessage);
      mCurrentGameState = gsm.getGameState();
      TRACE_REC("New Game State: " << gsm.getGameStateAsString());
    } else {
      TRACE_WARNING("Received unexpected message with ID : " << id);
    }
  }
  TRACE_FUNC_EXIT();
}

void Input::getLatestMousePosition() {
  TRACE_FUNC_ENTER();
  std::queue<sf::Packet> mouseMessageQueue = mMouseMessageSubscriber.getMessageQueue();
  sf::Packet mouseMessage;

  while (!mouseMessageQueue.empty()) {
    mouseMessage = mouseMessageQueue.front();
    mouseMessageQueue.pop();

    MouseMessage mm(mouseMessage);
    if (mm.getPosition() != sf::Vector2i()) {
      mLastMousePosition = mm.getPosition();
    }
  }
  TRACE_FUNC_EXIT();
}

void Input::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mInputThread->join();
  TRACE_INFO("Shutdown of module done");
}