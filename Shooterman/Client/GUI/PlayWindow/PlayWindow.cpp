#include "PlayWindow.h"

PlayWindow::PlayWindow() {
  mName = "CLIENT: PLAY_WINDOW";
  mSpriteManager = new SpriteManager();
  mSpriteManager->loadSprites();

  MessageHandler::get().subscribeToSpriteListMessages(&mSpriteListSubscriber);
}

PlayWindow::~PlayWindow() {
  MessageHandler::get().unsubscribeAll(&mSpriteListSubscriber);
  delete mSpriteManager;
}

bool PlayWindow::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  bool renderNeeded = false;
  std::queue<sf::Packet> spriteMessageQueue = mSpriteListSubscriber.getMessageQueue();
  sf::Packet spriteMessage;
  if (!spriteMessageQueue.empty()) {
    window->clear(sf::Color::White);
    renderNeeded = true;
  }

  while (!spriteMessageQueue.empty()) {
    spriteMessage = spriteMessageQueue.front();
    int messageID;
    spriteMessage >> messageID;
    if (messageID == SPRITE_LIST) {
      spriteMessageQueue.pop();
      SpriteMessage sm;
      sm.unpack(spriteMessage);
      int position = sm.getSize() - 1;
      std::pair<int, sf::Vector2f> spriteData = sm.getSpriteData(position);
      TRACE_DEBUG("SpriteID: " << spriteData.first);
      while (spriteData.first != -1) {
        TRACE_DEBUG(spriteData.first);
        sf::Sprite sprite = mSpriteManager->get(spriteData.first);
        sprite.setPosition(spriteData.second);
        window->draw(sprite);
        position--;
        spriteData = sm.getSpriteData(position);
      }
    }
    else {
      TRACE_DEBUG("Found no message");
    }
  }

  return renderNeeded;
}
