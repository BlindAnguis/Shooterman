#pragma once

#include <thread>
#include <map>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Resources/MenuBase.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/MessageHandler/Subscriber.h"
#include "../../Common/Trace.h"
#include "../../Common/Messages/GameStateMessage.h"

class Gui : Trace {
public:
  Gui();

  void shutDown();

private:
  bool mDebugEnabled1 = true;
  bool mWindowOpen;
  bool mF1KeyAlreadyPressed = false;
  std::unique_ptr<std::thread> mGuiThread;
  std::shared_ptr<sf::RenderWindow> mWindow;
  Subscriber mSystemMessageSubscriber;
  Subscriber mGameStateMessageSubscriber;
  Interface mMouseInterface;
  GAME_STATE mCurrentGameState;
  GAME_STATE mPreviousGameState;
  bool mRenderNeeded;
  bool mLeftButtonAlreadyPressed;
  std::map<GAME_STATE, std::list<std::shared_ptr<MenuBase>>> mMenuMap;
  sf::Clock mRenderClock;

  void run();
  void render();
  void handleWindowEvents();
  bool renderGameState(GAME_STATE gameState);
  void handleSystemMessages();
  void handleGameStateMessages();
};