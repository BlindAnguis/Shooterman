#pragma once

#include <thread>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>


#include "../../Common/KeyBindings.h"
#include "../../Common/MessageId.h"
#include "../../Common/Constants.h"
#include "../MessageHandler/MessageHandler.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/Messages/MouseMessage.h"
#include "../../Common/Messages/InputMessage.h"


class Input : Trace {
public:
	Input();

  void shutDown();

private:
  bool mRunning = true;
  std::unique_ptr<std::thread> mInputThread;
  Subscriber mSystemMessageSubscriber;
  Subscriber mGameStateMessageSubscriber;
  Subscriber mMouseMessageSubscriber;
  GAME_STATE mCurrentGameState;
  sf::Vector2i mLastMousePosition;

  void readInput();
  void handleSystemMessages();
  void handleGameStateMessages();
  void getLatestMousePosition();
};