#pragma once

#include "../Resources/MenuBase.h"
#include "../../../Common/Messages/SpriteCacheMessage.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class PlayWindow : public MenuBase {
public:
  PlayWindow(std::shared_ptr<MessageHandler> messageHandler);
  ~PlayWindow();

  void uninit() override;
  void reset() override;

  void backgroundUpdate() override;
  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  Subscriber mSpriteListSubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
  SpriteCacheMessage mSpriteListCacheMessage;
  SpriteMessage mSpriteListMessage;
  bool mIsRenderNeeded;
  sf::Packet mLatestSpriteMessage;
  std::vector<sf::Sprite> mCachedSprites;
  sf::RenderTexture mRenderTexture;
  sf::Sprite mCachedSprite;
  std::shared_ptr<sf::RenderWindow> mWindow;

  void renderSpriteData(std::shared_ptr<sf::RenderWindow> window, SpriteData& spriteData);
  void buildSpriteCache();

  void handleSpriteListMessage(sf::Packet& message);
  void handleSpriteListCacheMessage(sf::Packet& message);
};