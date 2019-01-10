#include "PlayWindow.h"

PlayWindow::PlayWindow() {
  mName = "CLIENT: PLAY_WINDOW";
  mSpriteManager = new SpriteManager();
  mSpriteManager->loadSprites();
  mIsSubscribed = false;
}

PlayWindow::~PlayWindow() {
  MessageHandler::get().unsubscribeTo("ClientSpriteList", &mSpriteListSubscriber);
  mIsSubscribed = false;
  delete mSpriteManager;
}

void PlayWindow::uninit() {
  MessageHandler::get().unsubscribeTo("ClientSpriteList", &mSpriteListSubscriber);
  mIsSubscribed = false;
  // This will clear the queue in case there is crap left over from previous game
  mSpriteListSubscriber.getMessageQueue();
}

bool PlayWindow::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  bool renderNeeded = false;

  if (!mIsSubscribed) {
    mIsSubscribed = MessageHandler::get().subscribeTo("ClientSpriteList", &mSpriteListSubscriber);
    if (!mIsSubscribed) {
      return false;
    }
  }

  std::queue<sf::Packet> spriteMessageQueue = mSpriteListSubscriber.getMessageQueue();
  sf::Packet spriteMessage;
  if (!spriteMessageQueue.empty()) {
    window->clear(sf::Color::White);
    renderNeeded = true;
  }

  while (!spriteMessageQueue.empty()) {
    spriteMessage = spriteMessageQueue.front();
    spriteMessageQueue.pop();
  }

  int messageID;
  spriteMessage >> messageID;
  if (messageID == SPRITE_LIST) {
    SpriteMessage sm;
    sm.unpack(spriteMessage);
    int position = sm.getSize() - 1;
    SpriteData spriteData = sm.getSpriteData(position);
    TRACE_DEBUG("SpriteID: " << static_cast<int>(spriteData.textureId));
    while (spriteData.textureId != Textures::Unknown) {
      TRACE_DEBUG(static_cast<int>(spriteData.textureId));
      sf::Sprite sprite = mSpriteManager->get(spriteData.textureId);
      sprite.setPosition(spriteData.position);
      sprite.setTextureRect(spriteData.texturePosition);
      sprite.setOrigin((float)(sprite.getTextureRect().width / 2), 
                       (float)(sprite.getTextureRect().height / 2));
      sprite.setRotation(spriteData.rotation);
      window->draw(sprite);
      position--;
      spriteData = sm.getSpriteData(position);
    }
  } else {
    TRACE_DEBUG("Found no message");
  }

  return renderNeeded;
}
