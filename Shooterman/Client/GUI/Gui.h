#pragma once

#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "SpriteManager.h"
#include "MainMenu/MainMenu.h"
#include "LobbyMenu/LobbyMenu.h"
#include "../MessageHandler/MessageHandler.h"
#include "../../Common/Subscriber.h"
#include "../../Common/MessageId.h"
#include "../../Common/Trace.h"
#include "../../Common/Constants.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/GameStateMessage.h"

class Gui : Trace {
public:
  Gui();

  void shutDown();

private:
  bool mDebugEnabled = true;
  std::thread* mGuiThread;
  sf::RenderWindow* mWindow;
  Subscriber mSystemMessageSubscriber;
  Subscriber mSpriteListSubscriber;
  SpriteManager* mSpriteManager;
  GAME_STATE mCurrentGameState;
  bool mRenderNeeded;
  bool mLeftButtonAlreadyPressed;
  MainMenu mMainMenu;
  LobbyMenu mLobbyMenu;

  void init();
  void render();
  void handleSystemMessages();
  void mainMenu();
  void lobbyMenu();
  void playing();
};