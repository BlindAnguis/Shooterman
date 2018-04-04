#pragma once
#include <thread>
#include "Client/MessageHandler/MessageHandler.h"
#include "Common/KeyBindings.h"

class GameLoop {
public:
  GameLoop(MessageHandler* messageHandler);
  GameLoop();
  void start();
  void stop();

private: 
  bool mRunning;
  std::thread* mGameLoopThread;
  MessageHandler* mMessageHandler;
  void gameLoop();
};

enum class GameLoopState { 
  LOBBY, 
  SETUP_GAME, 
  PLAYING, 
  GAME_OVER,
  EXIT
};