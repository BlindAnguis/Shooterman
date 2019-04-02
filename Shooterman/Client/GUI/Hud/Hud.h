#pragma once

#include "../Resources/MenuBase.h"

class Hud : public MenuBase {
public:
  Hud();
  ~Hud();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  bool mSubscribedToPlayerData;
  Subscriber mPlayerDataSubscriber;
  std::shared_ptr<GUIComponent> mHealthNumber;
};