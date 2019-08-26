#pragma once

#include <stack>

#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/Trace.h"

class ClientMain : Trace {
public:
	ClientMain();

private:
  bool mRunning = true;
  bool mServerStarted;
  Interface mGameStateInterface;
  Interface mSystemMessageInterface;
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::stack<GAME_STATE> mGameStateStack;

  void run();
  void handleChangeGameStateMessage(sf::Packet& message);
  void handleShutdownMessage(sf::Packet& message);
};