#pragma once

#include <stack>

#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/Trace.h"

class ClientMain : Trace {
public:
	ClientMain();

private:
  bool mRunning = true;
  Interface mGameStateInterface;
  Interface mSystemMessageInterface;
  bool mServerStarted;
  std::stack<GAME_STATE> mGameStateStack;

  void run();
  void handleChangeGameStateMessage(sf::Packet& message);
  void handleShutdownMessage(sf::Packet& message);
};