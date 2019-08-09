#include "PlayWindow.h"

#include "../Resources/GuiResourceManager.h"
#include "../../../Common/Messages/SpriteMessage.h"
#include "../../../Common/Interfaces.h"

PlayWindow::PlayWindow() {
  mName = "CLIENT: PLAY_WINDOW";
  mGuiFrame = std::make_shared<Frame>();

  mSpriteListSubscriber.addSignalCallback(MessageId::SPRITE_LIST_CACHE, std::bind(&PlayWindow::handleSpriteListCacheMessage, this, std::placeholders::_1));
  mSpriteListSubscriber.addSignalCallback(MessageId::SPRITE_LIST, std::bind(&PlayWindow::handleSpriteListMessage, this, std::placeholders::_1));
  MessageHandler::get().subscribeToWithTimeout(Interfaces::CLIENT_SPRITE_LIST, &mSpriteListSubscriber);

  setupDebugMessages("Client", "Play Window");
}

PlayWindow::~PlayWindow() {
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_SPRITE_LIST, &mSpriteListSubscriber);
}

void PlayWindow::uninit() {
  mSpriteListSubscriber.clearMessages();
}

void PlayWindow::reset() {
  mCachedSprites.clear();
}

void PlayWindow::backgroundUpdate() {
  handleDebugMessages();
}

bool PlayWindow::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  mWindow = window;
  mIsRenderNeeded = false;

  mSpriteListSubscriber.handleMessages();

  return mIsRenderNeeded;
}

void PlayWindow::renderSpriteData(std::shared_ptr<sf::RenderWindow> window, SpriteData& spriteData) {
  //TRACE_DEBUG1(static_cast<int>(spriteData.textureId));
  sf::Sprite sprite = GuiResourceManager::getInstance().createSprite(spriteData.textureId);
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
    sprite = GuiResourceManager::getInstance().createSprite(spriteData.textureId);
    sprite.setPosition(spriteData.position);
    sprite.setTextureRect(spriteData.texturePosition);
    sprite.setOrigin((float)(sprite.getTextureRect().width / 2),
      (float)(sprite.getTextureRect().height / 2));
    sprite.setRotation(spriteData.rotation);
    mCachedSprites.push_back(sprite);
    cachedSpriteListPosition--;
    spriteData = mSpriteListCacheMessage.getSpriteData(cachedSpriteListPosition);
  }

  if (!mRenderTexture.create(1024, 1024)) {
    TRACE_ERROR("Could not create Render Texture!");
    return;
  }

  mRenderTexture.clear();
  for (auto sprite : mCachedSprites) {
    mRenderTexture.draw(sprite);
  }
  mRenderTexture.display();

  mCachedSprite = sf::Sprite(mRenderTexture.getTexture());
}

void PlayWindow::handleSpriteListMessage(sf::Packet& message) {
  mWindow->clear(sf::Color::White);
  mIsRenderNeeded = true;

  mWindow->draw(mCachedSprite);

  mSpriteListMessage.unpack(message);
  int position = mSpriteListMessage.getSize() - 1;
  SpriteData spriteData = mSpriteListMessage.getSpriteData(position);
  //TRACE_DEBUG1("SpriteID: " << static_cast<int>(spriteData.textureId));
  while (spriteData.textureId != Textures::Unknown) {
    renderSpriteData(mWindow, spriteData);
    position--;
    spriteData = mSpriteListMessage.getSpriteData(position);
  }
}

void PlayWindow::handleSpriteListCacheMessage(sf::Packet& message) {
  mCachedSprites.clear();
  mSpriteListCacheMessage.unpack(message);
  buildSpriteCache();
}
