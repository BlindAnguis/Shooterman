#pragma once

#include <thread>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "SpriteManager.h"
#include "Resources/MenuBase.h"
#include "MainMenu/MainMenu.h"
#include "LobbyMenu/LobbyMenu.h"
#include "JoinMenu/JoinMenu.h"
#include "PlayWindow/PlayWindow.h"
#include "OptionsMenu/OptionsMenu.h" 
#include "DebugMenu/DebugMenu.h"
#include "PauseMenu/PauseMenu.h"

#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/MessageHandler/Subscriber.h"
#include "../../Common/MessageId.h"
#include "../../Common/Trace.h"
#include "../../Common/Constants.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/Messages/MouseMessage.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"

class Gui : Trace {
public:
  Gui();

  void shutDown();

private:
  bool mDebugEnabled = true;
  bool mWindowOpen;
  bool mShowDebugMenu;
  bool mF1KeyAlreadyPressed = false;
  std::unique_ptr<std::thread> mGuiThread;
  std::shared_ptr<sf::RenderWindow> mWindow;
  Subscriber mSystemMessageSubscriber;
  Subscriber mGameStateMessageSubscriber;
  Subscriber mDebugSubscriber;
  Interface* mMouseInterface;
  GAME_STATE mCurrentGameState;
  bool mRenderNeeded;
  bool mLeftButtonAlreadyPressed;
  std::map<GAME_STATE, MenuBase*> mMenuMap;
  MenuBase* mDebugMenu;
  sf::Clock mRenderClock;

  void init();
  void render();
  void handleWindowEvents();
  bool renderGameState(GAME_STATE gameState);
  void handleSystemMessages();
  void handleGameStateMessages();
  void handleDebugMessages();
};