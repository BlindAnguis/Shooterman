#pragma once
#include <thread>
#include <memory>

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
  std::shared_ptr<NetworkSystem> mNetworkSystem;
  std::shared_ptr<MessageHandler> mMessageHandler;

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