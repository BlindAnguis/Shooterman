#ifndef GUI_H
#define GUI_H

#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "SpriteManager.h"
#include "../MessageHandler/MessageHandler.h"
#include "../../Common/Subscriber.h"
#include "../../Common/MessageId.h"

class Gui {
public:
  Gui();

  void shutDown();

private:
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

#endif // GUI_H