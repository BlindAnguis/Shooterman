#pragma once
#include <thread>
#include "../../Client/MessageHandler/MessageHandler.h"
#include "../../Common/KeyBindings.h"
#include "../../Common/Trace.h"
#include "../../Common/Constants.h"

class GameLoop : Trace {
public:
  GameLoop();
  void start();
  void stop();

private:
  bool mRunning;
  std::thread* mGameLoopThread;
  void gameLoop();
};

enum class GameLoopState { 
  LOBBY, 
  SETUP_GAME, 
  PLAYING, 
  GAME_OVER,
  EXIT
};