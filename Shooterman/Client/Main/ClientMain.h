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
  Interface gameStateInterface;
  Interface systemMessageInterface;
  bool mServerStarted;
  std::stack<GAME_STATE> mGameStateStack;

  void handleSystemMessages();
  void handleGameStateMessages();
};