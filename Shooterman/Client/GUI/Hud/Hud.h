#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiProgressBar.h"
#include "../Resources/GuiText.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class Hud : public MenuBase {
public:
  Hud();
  ~Hud();

  void reset() override;

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  bool mSubscribedToPlayerData = false;
  Subscriber mSubscriber;
  std::vector<std::shared_ptr<GuiText>> mUsernames;
  std::vector<std::shared_ptr<GuiProgressBar>> mHealthBars;
  std::map<unsigned int, std::shared_ptr<GuiProgressBar>> mStaminaBars;
  std::map<unsigned int, std::shared_ptr<GuiProgressBar>> mManaBars;

  void handlePlayerData(sf::Packet& message);
};