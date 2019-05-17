#include "PlayWindow.h"

PlayWindow::PlayWindow() {
  mName = "CLIENT: PLAY_WINDOW";
  mSpriteManager = new SpriteManager();
  mSpriteManager->loadSprites();
  mIsSubscribed = false;
  mGuiFrame = std::make_shared<Frame>();
}

PlayWindow::~PlayWindow() {
  MessageHandler::get().unsubscribeTo("ClientSpriteList", &mSpriteListSubscriber);
  mIsSubscribed = false;
  delete mSpriteManager;
}

void PlayWindow::uninit() {
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

  if (!spriteMessageQueue.empty()) {
    window->clear(sf::Color::White);
    renderNeeded = true;
  }

  sf::Packet spriteMessage;
  int messageID = 0;
  while (!spriteMessageQueue.empty()) {
    spriteMessage = spriteMessageQueue.front();
    spriteMessageQueue.pop();
    spriteMessage >> messageID;
    if (messageID == SPRITE_LIST_CACHE) {
      mCachedSprites.clear();
      mSpriteListCacheMessage.unpack(spriteMessage);
      buildSpriteCache();
    } else if (messageID == SPRITE_LIST) {
      mSpriteListMessage.unpack(spriteMessage);
    }
  }

  if (messageID == SPRITE_LIST) {
    for (auto cachedSprite : mCachedSprites) {
      window->draw(cachedSprite);
    }

    int position = mSpriteListMessage.getSize() - 1;
    SpriteData spriteData = mSpriteListMessage.getSpriteData(position);
    //TRACE_DEBUG("SpriteID: " << static_cast<int>(spriteData.textureId));
    while (spriteData.textureId != Textures::Unknown) {
      renderSpriteData(window, spriteData);
      position--;
      spriteData = mSpriteListMessage.getSpriteData(position);
    }
  } else {
    TRACE_DEBUG("Found no message");
  }
  return renderNeeded;
}

void PlayWindow::renderSpriteData(std::shared_ptr<sf::RenderWindow> window, SpriteData& spriteData) {
  //TRACE_DEBUG(static_cast<int>(spriteData.textureId));
  sf::Sprite sprite = mSpriteManager->get(spriteData.textureId);
  sprite.setPosition(spriteData.position);
  sprite.setTextureRect(spriteData.texturePosition);
  sprite.setOrigin((float)(sprite.getTextureRect().width / 2),
    (float)(sprite.getTextureRect().height / 2));
  sprite.setRotation(spriteData.rotation);
  window->draw(sprite);
}

void PlayWindow::buildSpriteCache() {
  int cachedSpriteListPosition = mSpriteListCacheMessage.getSize() - 1;
  SpriteData spriteData = mSpriteListCacheMessage.getSpriteData(cachedSpriteListPosition);
  while (spriteData.textureId != Textures::Unknown) {
    sf::Sprite sprite;
    sprite = mSpriteManager->get(spriteData.textureId);
    sprite.setPosition(spriteData.position);
    sprite.setTextureRect(spriteData.texturePosition);
    sprite.setOrigin((float)(sprite.getTextureRect().width / 2),
      (float)(sprite.getTextureRect().height / 2));
    sprite.setRotation(spriteData.rotation);
    mCachedSprites.push_back(sprite);
    cachedSpriteListPosition--;
    spriteData = mSpriteListCacheMessage.getSpriteData(cachedSpriteListPosition);
  }
}