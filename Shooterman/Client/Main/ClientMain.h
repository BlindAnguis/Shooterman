#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "../Input/Input.h"
#include "../MessageHandler/MessageHandler.h"
#include "../GUI/Gui.h"
#include "../Sound/Sound.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/Trace.h"
#include "../../Server/Main/GameLoop.h"

class ClientMain : Trace {
public:
	ClientMain();

private:
  bool mRunning = true;
  Subscriber mSystemMessageSubscriber;
  bool mServerStarted;
  GAME_STATE mCurrentGameState;

  void handleSystemMessages();
};