#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiProgressBar.h"
#include "../Resources/GuiText.h"
#include "../Resources/GuiList.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class Hud : public MenuBase {
public:
  Hud(std::shared_ptr<MessageHandler> messageHandler);
  ~Hud();

  void reset() override;

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  Subscriber mSubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::vector<std::shared_ptr<GuiText>> mUsernames;
  std::vector<std::shared_ptr<GuiProgressBar>> mHealthBars;
  std::map<unsigned int, std::shared_ptr<GuiProgressBar>> mStaminaBars;
  std::map<unsigned int, std::shared_ptr<GuiProgressBar>> mManaBars;
  std::vector<std::shared_ptr<GuiText>> mScores;
  std::shared_ptr<GuiList> mScoreList;

  void handlePlayerData(sf::Packet& message);
};