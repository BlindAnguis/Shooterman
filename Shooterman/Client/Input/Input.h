#pragma once

#include <thread>

#include "../../Common/Trace.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/Messages/GameStateMessage.h"

class Input : Trace {
public:
	Input();
  ~Input();
  void shutDown();

private:
  bool mRunning = true;
  bool recentlyChangedState = true;
  std::unique_ptr<std::thread> mInputThread;
  Subscriber mSystemMessageSubscriber;
  Subscriber mGameStateMessageSubscriber;
  Subscriber mMouseMessageSubscriber;
  Interface mClientInputInterface;
  GAME_STATE mCurrentGameState;
  sf::Vector2i mLastMousePosition;

  void run();
  void handleSystemMessages();
  void handleGameStateMessages();
  void getLatestMousePosition();
};