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
#include "InfoOverlay/InfoOverlay.h"

class Gui : Trace {
public:
  Gui(std::shared_ptr<MessageHandler> messageHandler);

  void shutDown();

private:
  bool mDebugEnabled1 = true;
  bool mWindowOpen;
  bool mF1KeyAlreadyPressed = false;
  std::unique_ptr<std::thread> mGuiThread;
  std::shared_ptr<sf::RenderWindow> mWindow;
  Subscriber mSubscriber;
  Interface mMouseInterface;
  std::shared_ptr<MessageHandler> mMessageHandler;
  GAME_STATE mCurrentGameState;
  GAME_STATE mPreviousGameState;
  bool mRenderNeeded;
  bool mLeftButtonAlreadyPressed;
  std::map<GAME_STATE, std::list<std::shared_ptr<MenuBase>>> mMenuMap;
  std::shared_ptr<InfoOverlay> mInfoOverlay;
  sf::Clock mRenderClock;

  void run();
  void render();
  void handleWindowEvents();
  bool renderGameState(GAME_STATE gameState);
  void handleShutdownMessage(sf::Packet& message);
  void handleChangedGameStateMessage(sf::Packet& message);
};