#pragma once
#include "../Resources/MenuBase.h"
#include "../Resources/GuiList.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common/MessageHandler/Subscriber.h"

class ScoreMenu : public MenuBase {
public:
  ScoreMenu(std::shared_ptr<MessageHandler> messageHandler);
  ~ScoreMenu();

  void backgroundUpdate() override;

private:
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::shared_ptr<GuiList> mScoreList;
  Subscriber mScoreBoardSubscriber;

  void handleScoreBoardMessage(sf::Packet& packet);

  void onMainMenuClick();
  void onLobbyMenuClick();
};

