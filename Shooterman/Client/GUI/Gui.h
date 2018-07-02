#pragma once

#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "SpriteManager.h"
#include "../MessageHandler/MessageHandler.h"
#include "../../Common/Subscriber.h"
#include "../../Common/MessageId.h"
#include "../../Common/Trace.h"
#include "../../Common/Constants.h"
#include "../../Common/Messages/SpriteMessage.h"

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
  sf::Sprite mSprite;

  void init();
  void render();
  void handleSystemMessages();
};