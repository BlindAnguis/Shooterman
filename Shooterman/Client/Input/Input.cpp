#include "Input.h"

#include "../../Common/KeyBindings.h"
#include "../../Common/Constants.h"
#include "../../Common/Interfaces.h"
#include "../../Common/Messages/MouseMessage.h"
#include "../../Common/Messages/InputMessage.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"

Input::Input(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<Keyboard> keyboard) : mMessageHandler(messageHandler), mSubscriber("INPUT"), mKeyboard(keyboard) {
  mName = "CLIENT: INPUT";
}
Input::~Input() { }

void Input::start() {
  TRACE_INFO("Starting module...");
  mCurrentGameState = GAME_STATE::MAIN_MENU;

  mGameStateMessageSubscriber.addSignalCallback(MessageId::CHANGE_GAME_STATE, std::bind(&Input::handleChangeGameStateMessage, this, std::placeholders::_1));
  mSubscriber.addSignalCallback(MessageId::MOUSE_MESSAGE, std::bind(&Input::handleMousePositionMessage, this, std::placeholders::_1));

  mMessageHandler->subscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &mSubscriber);
  mMessageHandler->subscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateMessageSubscriber);
  mMessageHandler->subscribeToWithTimeout(Interfaces::MOUSE_POSITION, &mSubscriber);
  mMessageHandler->publishInterface(Interfaces::CLIENT_INPUT_LIST, &mClientInputInterface);
  
  setupDebugMessages("Client", "Input", mMessageHandler);
  startListenToSubscriber(&mSubscriber);
  startListenToSubscriber(&mGameStateMessageSubscriber);
  TRACE_INFO("Starting module done");
}

void Input::run() {
  TRACE_FUNC_ENTER();
  std::uint32_t keyboardBitmask;
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
      if (mKeyboard->isKeyPressed(KEY::ESC) && recentlyChangedState) {
        break;
      }
      recentlyChangedState = false;
      if (mKeyboard->isKeyPressed(KEY::ESC)) {
        GameStateMessage gsm(GAME_STATE::PLAYING);
        TRACE_SEND("New Game State: PLAYING");
		    mGameStateMessageSubscriber.reverseSendMessage(gsm.pack());
        recentlyChangedState = true;
      }
      break;
    case GAME_STATE::PLAYING: {
      if (mKeyboard->isKeyPressed(KEY::ESC) && recentlyChangedState) {
        break;
      }
      recentlyChangedState = false;
      keyboardBitmask = 0;
      if (mKeyboard->isKeyPressed(KEY::A) || mKeyboard->isKeyPressed(KEY::LEFT)) {
        keyboardBitmask |= A_KEY;
      }
      if (mKeyboard->isKeyPressed(KEY::D) || mKeyboard->isKeyPressed(KEY::RIGHT)) {
        keyboardBitmask |= D_KEY;
      }
      if (mKeyboard->isKeyPressed(KEY::W) || mKeyboard->isKeyPressed(KEY::UP)) {
        keyboardBitmask |= W_KEY;
      }
      if (mKeyboard->isKeyPressed(KEY::S) || mKeyboard->isKeyPressed(KEY::DOWN)) {
        keyboardBitmask |= S_KEY;
      }
      if (mKeyboard->isKeyPressed(KEY::LEFT_MOUSE)) {
        keyboardBitmask |= LEFT_MOUSE_VALUE;
      }
      if (mKeyboard->isKeyPressed(KEY::RIGHT_MOUSE)) {
        keyboardBitmask |= RIGHT_MOUSE_VALUE;
      }

      InputMessage im(keyboardBitmask, mLastMousePosition.x, mLastMousePosition.y);
      mClientInputInterface.pushMessage(im.pack());

      if (mKeyboard->isKeyPressed(KEY::ESC)) {
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

  handleDebugMessages();

  TRACE_FUNC_EXIT();
}

void Input::stop() {
  TRACE_INFO("Shutdown of module requested...");
  teardownDebugMessages();
  mMessageHandler->unpublishInterface(Interfaces::CLIENT_INPUT_LIST);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &mSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_INPUT_LIST, &mSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateMessageSubscriber);
  TRACE_INFO("Shutdown of module done");
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