#include "Input.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../../Common/KeyBindings.h"
#include "../../Common/Constants.h"
#include "../../Common/Interfaces.h"
#include "../../Common/Messages/MouseMessage.h"
#include "../../Common/Messages/InputMessage.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"

Input::Input(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler) {
  mName = "CLIENT: INPUT";
  TRACE_INFO("Starting module...");
  mInputThread = std::make_unique<std::thread>(&Input::run, this);
  TRACE_INFO("Starting module done");
}
Input::~Input() {
  TRACE_FUNC_ENTER();
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateMessageSubscriber);
  TRACE_FUNC_EXIT();
}

void Input::run() {
  TRACE_FUNC_ENTER();
  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mRunning = true;

  mSubscriber.addSignalCallback(MessageId::SHUT_DOWN, std::bind(&Input::handleShutdownMessage, this, std::placeholders::_1));
  mGameStateMessageSubscriber.addSignalCallback(MessageId::CHANGE_GAME_STATE, std::bind(&Input::handleChangeGameStateMessage, this, std::placeholders::_1));
  mSubscriber.addSignalCallback(MessageId::MOUSE_MESSAGE, std::bind(&Input::handleMousePositionMessage, this, std::placeholders::_1));

  mMessageHandler->subscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &mSubscriber);
  mMessageHandler->subscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateMessageSubscriber);
  mMessageHandler->subscribeToWithTimeout(Interfaces::MOUSE_POSITION, &mSubscriber);
  mMessageHandler->publishInterface(Interfaces::CLIENT_INPUT_LIST, &mClientInputInterface);
  setupDebugMessages("Client", "Input", mMessageHandler);

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
      case GAME_STATE::MAP_EDITOR:
        break;
      default:
        TRACE_ERROR("Unknown game state: " << mCurrentGameState);
    }

    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
    mSubscriber.handleMessages();
    mGameStateMessageSubscriber.handleMessages();

    handleDebugMessages();
  }

  teardownDebugMessages();
  mMessageHandler->unpublishInterface(Interfaces::CLIENT_INPUT_LIST);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &mSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_INPUT_LIST, &mSubscriber);
  TRACE_FUNC_EXIT();
}

void Input::handleShutdownMessage(sf::Packet& message) {
  TRACE_FUNC_ENTER();
  mRunning = false;
  TRACE_INFO("Preparing to shut down");
  TRACE_FUNC_EXIT();
}

void Input::handleChangeGameStateMessage(sf::Packet& message) {
  TRACE_FUNC_ENTER();
  GameStateMessage gsm(message);
  mCurrentGameState = gsm.getGameState();
  TRACE_REC("New Game State: " << gsm.getGameStateAsString());
  TRACE_FUNC_EXIT();
}

void Input::handleMousePositionMessage(sf::Packet& message) {
  TRACE_FUNC_ENTER();
  MouseMessage mm(message);
  if (mm.getPosition() != sf::Vector2i()) {
    mLastMousePosition = mm.getPosition();
  }
  TRACE_FUNC_EXIT();
}

void Input::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mInputThread->join();
  TRACE_INFO("Shutdown of module done");
}