#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiList.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class LobbyMenu : public MenuBase {
public:
  LobbyMenu(bool server);
  ~LobbyMenu();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  bool mSubscribedToLobby;
  Subscriber mLobbySubscriber;
  std::shared_ptr<GuiList> mPlayersList;

  void checkForLobbyMessages();
};