#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class LobbyMenu : public MenuBase {
public:
  LobbyMenu(bool server);
  ~LobbyMenu();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  bool mSubscribedToLobby;
  bool mSubscribedToServerReady = false;
  Subscriber mLobbySubscriber;
  Subscriber mServerReadySubscriber;
  std::shared_ptr<GuiList> mPlayersList;
  std::shared_ptr<GuiButton> mStartGameButton;
  bool mServer;

  void checkForLobbyMessages();
  void checkForServerReadyMessage();
};