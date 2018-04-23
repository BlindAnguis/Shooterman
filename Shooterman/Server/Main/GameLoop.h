#pragma once
#include <thread>
#include "Client/MessageHandler/MessageHandler.h"
#include "Common/KeyBindings.h"

class GameLoop {
public:
  GameLoop();
  void start();
  void stop();

private: 
  bool mRunning;
  std::thread* mGameLoopThread;
  Subscriber mInputSubscriber;
  void gameLoop();
};

enum class GameLoopState { 
  LOBBY, 
  SETUP_GAME, 
  PLAYING, 
  GAME_OVER,
  EXIT
};