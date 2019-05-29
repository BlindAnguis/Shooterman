#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiProgressBar.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class Hud : public MenuBase {
public:
  Hud();
  ~Hud();

  void reset() override;

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  bool mSubscribedToPlayerData = false;
  Subscriber mPlayerDataSubscriber;
  std::vector<std::shared_ptr<GuiProgressBar>> mHealthBars;
  std::vector<std::shared_ptr<GuiProgressBar>> mStaminaBars;
  std::vector<std::shared_ptr<GuiProgressBar>> mManaBars;
};