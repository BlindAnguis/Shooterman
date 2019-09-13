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
#include "../../Common/Process/Runnable.h"
#include "InfoOverlay/InfoOverlay.h"

class Gui : public Runnable, public Trace {
public:
  Gui(std::shared_ptr<MessageHandler> messageHandler);

  void start() override;
  void run() override;
  void stop() override;

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

  void handleWindowEvents();
  bool renderGameState(GAME_STATE gameState);
  void handleShutdownMessage(sf::Packet& message);
  void handleChangedGameStateMessage(sf::Packet& message);
};