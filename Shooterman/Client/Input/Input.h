#pragma once

#include <thread>

#include "Keyboard.h"
#include "../../Common/Trace.h"
#include "../../Common/Process/Runnable.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/Messages/GameStateMessage.h"

class Input : public Runnable, public Trace {
public:
	Input(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<Keyboard> keyboard);
  ~Input();

  void start() override;
  void run() override;
  void stop() override;

private:
  bool recentlyChangedState = true;
  std::unique_ptr<std::thread> mInputThread;
  std::shared_ptr<Keyboard> mKeyboard;
  Subscriber mSubscriber;
  Subscriber mGameStateMessageSubscriber;
  Interface mClientInputInterface;
  std::shared_ptr<MessageHandler> mMessageHandler;
  GAME_STATE mCurrentGameState;
  sf::Vector2i mLastMousePosition;

  void handleChangeGameStateMessage(sf::Packet& message);
  void handleMousePositionMessage(sf::Packet& message);
};