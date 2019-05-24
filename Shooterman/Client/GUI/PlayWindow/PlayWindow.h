#pragma once

#include "../Resources/MenuBase.h"
#include "../SpriteManager.h"
#include "../../../Common/Messages/SpriteMessage.h"
#include "../../../Common/Messages/SpriteCacheMessage.h"

class PlayWindow : public MenuBase {
public:
  PlayWindow();
  ~PlayWindow();

  void uninit() override;
  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  SpriteManager mSpriteManager;
  Subscriber mSpriteListSubscriber;
  SpriteCacheMessage mSpriteListCacheMessage;
  SpriteMessage mSpriteListMessage;
  bool mIsSubscribed;
  sf::Packet mLatestSpriteMessage;
  std::vector<sf::Sprite> mCachedSprites;

  void renderSpriteData(std::shared_ptr<sf::RenderWindow> window, SpriteData& spriteData);
  void buildSpriteCache();
};