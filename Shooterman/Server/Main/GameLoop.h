#pragma once
#include <thread>
#include "../Systems/NetworkSystem/NetworkSystem.h"
#include "../../Common/Trace.h"
#include "../../Common/MessageHandler/MessageHandler.h"

class GameLoop : Trace {
public:
  GameLoop();
  void start();
  void stop();

private:
  bool mRunning;
  std::thread* mGameLoopThread;
  NetworkSystem* mNetworkSystem;

  void gameLoop();
  void cleanUp();
};

enum class GameLoopState { 
  LOBBY, 
  SETUP_GAME, 
  PLAYING, 
  GAME_OVER,
  EXIT
};