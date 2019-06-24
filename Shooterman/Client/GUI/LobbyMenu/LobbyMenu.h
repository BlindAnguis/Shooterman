#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../Resources/GuiInputText.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class LobbyMenu : public MenuBase {
public:
  LobbyMenu(bool server);
  ~LobbyMenu();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;
  void handleNewText(sf::Uint32 newChar);

  void init() override;

private:
  bool mServer;
  bool mSubscribedToLobby;
  bool mSubscribedToServerReady = false;
  std::string mUsername;
  Subscriber mLobbySubscriber;
  Subscriber mServerReadySubscriber;
  std::shared_ptr<GuiList> mPlayersList;
  std::shared_ptr<GuiList> mPlayableCharactersList;
  std::shared_ptr<GuiList> mFileList;
  std::list<std::shared_ptr<GuiButton>> mFileButtonList;
  std::shared_ptr<GuiButton> mStartGameButton;
  std::shared_ptr<GuiInputText> mUsernameText;

  void checkForLobbyMessages();
  void checkForServerReadyMessage();
};