#pragma once

#include <thread>

#include "../../Common/Trace.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/Messages/GameStateMessage.h"

class Input : Trace {
public:
	Input(std::shared_ptr<MessageHandler> messageHandler);
  ~Input();
  void shutDown();

private:
  bool mRunning;
  bool recentlyChangedState = true;
  std::unique_ptr<std::thread> mInputThread;
  Subscriber mSubscriber;
  Subscriber mGameStateMessageSubscriber;
  Interface mClientInputInterface;
  std::shared_ptr<MessageHandler> mMessageHandler;
  GAME_STATE mCurrentGameState;
  sf::Vector2i mLastMousePosition;

  void run();

  void handleShutdownMessage(sf::Packet& message);
  void handleChangeGameStateMessage(sf::Packet& message);
  void handleMousePositionMessage(sf::Packet& message);
};