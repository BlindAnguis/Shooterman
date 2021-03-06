#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../Resources/GuiInputText.h"
#include "../Resources/GuiImageButton.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class LobbyMenu : public MenuBase {
public:
  LobbyMenu(bool server, std::shared_ptr<MessageHandler> messageHandler);
  ~LobbyMenu();

  void backgroundUpdate() override;
  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

  void init() override;

private:
  bool mServer;
  bool mSubscribedToLobby;
  bool mSubscribedToServerReady = false;
  Subscriber mLobbySubscriber;
  Subscriber mServerReadySubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::shared_ptr<GuiList> mPlayersList;
  std::shared_ptr<GuiList> mPlayableCharactersList;
  std::shared_ptr<GuiList> mFileList;
  std::list<std::shared_ptr<GuiButton>> mFileButtonList;
  std::shared_ptr<GuiButton> mStartGameButton;
  std::shared_ptr<GuiInputText> mUsernameText;
  std::vector<std::string> mLevelDirectories;
  sf::Texture mThumbnailTexture;
  sf::Sprite mThumbnail;
  std::shared_ptr<GuiImageButton> mThumbnailImage;

  void handleServerSubscribeTimeout(sf::Packet& message);
  void handleServerReadyMessage(sf::Packet& message);
  void handleLobbySubscribeDoneMessage(sf::Packet& message);
  void handleLobbySubscribeTimeout(sf::Packet& message);
  void handlePlayerUsernameMessage(sf::Packet& message);
  void handlePlayableCharactersMessage(sf::Packet& message);
  void handleMapMessage(sf::Packet& message);

  void onChangeUsernameClick();
  void onUsernameSelect();
};