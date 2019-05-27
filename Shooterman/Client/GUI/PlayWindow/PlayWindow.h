#pragma once

#include "../Resources/MenuBase.h"
#include "../../../Common/Messages/SpriteCacheMessage.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class PlayWindow : public MenuBase {
public:
  PlayWindow();
  ~PlayWindow();

  void uninit() override;
  void reset() override;

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  Subscriber mSpriteListSubscriber;
  SpriteCacheMessage mSpriteListCacheMessage;
  SpriteMessage mSpriteListMessage;
  bool mIsSubscribed;
  sf::Packet mLatestSpriteMessage;
  std::vector<sf::Sprite> mCachedSprites;

  void renderSpriteData(std::shared_ptr<sf::RenderWindow> window, SpriteData& spriteData);
  void buildSpriteCache();
};