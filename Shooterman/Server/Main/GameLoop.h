#pragma once
#include <thread>
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/KeyBindings.h"
#include "../../Common/Trace.h"
#include "../../Common/Constants.h"
#include "../Systems/NetworkSystem/NetworkSystem.h"
#include "../../Common/Socket/Tcp.h"

class GameLoop : Trace {
public:
  GameLoop();
  void start();
  void stop();

private:
  bool mRunning;
  std::thread* mGameLoopThread;
  std::shared_ptr<NetworkSystem> mNetworkSystem;

  void gameLoop();
};

enum class GameLoopState { 
  LOBBY, 
  SETUP_GAME, 
  PLAYING, 
  GAME_OVER,
  EXIT
};