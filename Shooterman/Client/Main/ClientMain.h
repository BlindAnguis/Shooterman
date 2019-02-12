#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <stack>
#include <time.h>

#include "Executor.h"
#include "Tasks/Network/SetupNetworkHandlerCommunicationTask.h"
#include "../Input/Input.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../GUI/Gui.h"
#include "../Sound/Sound.h"
#include "../Network/NetworkHandler.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/Trace.h"
#include "../../Server/Main/GameLoop.h"

class ClientMain : Trace {
public:
	ClientMain();

private:
  bool mRunning = true;
  Interface gameStateInterface;
  Subscriber mSystemMessageSubscriber;
  Subscriber mGameStateMessageSubscriber;
  bool mServerStarted;
  Executor mExecutor;
  std::stack<GAME_STATE> mGameStateStack;

  void handleSystemMessages();
  void handleGameStateMessages();
};