#include "LobbyMenu.h"

#include "../Resources/GuiComponentFactory.h"
#include "../Resources/GuiText.h"
#include "../../../Common/Messages/LobbyDataMessage.h"
#include "../../../Common/MessageHandler/Subscriber.h"

LobbyMenu::LobbyMenu(bool server) {
  mName = "CLIENT: LOBBY_MENU";

  mGuiFrame = std::make_shared<Frame>();

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Join Lobby"));

  mPlayersList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::VERTICAL);
  mPlayersList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, "Dummy value 1"));
  mPlayersList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, "Dummy value 2"));
  mGuiFrame->addGuiComponent(mPlayersList);

  auto lobbyMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);
  if (server) {
    lobbyMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Start Game", GAME_STATE::PLAYING));
  }
  lobbyMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::PREVIOUS));

  mGuiFrame->addGuiComponent(lobbyMenuList);

  mSubscribedToLobby = false;

}

LobbyMenu::~LobbyMenu() { }

bool LobbyMenu::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  checkForLobbyMessages();
  mGuiFrame->render(window);
  return true;
}

void LobbyMenu::checkForLobbyMessages() {
  if (!mSubscribedToLobby) {
    mSubscribedToLobby = MessageHandler::get().subscribeTo("ClientLobby", &mLobbySubscriber);
  }

  auto messages = mLobbySubscriber.getMessageQueue();

  sf::Packet lobbyMessage;
  int messageID = 0;
  while (!messages.empty()) {
    lobbyMessage = messages.front();
    messages.pop();
    lobbyMessage >> messageID;
    if (messageID == LOBBY_DATA) {
      LobbyDataMessage ldm;
      ldm.unpack(lobbyMessage);


      mPlayersList->clear();
      for (auto playerName : ldm.getPlayerNames()) {
        mPlayersList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, playerName));
      }

    } else if (messageID == SPRITE_LIST) {
      TRACE_WARNING("Received unhandeled message: " << messageID);
    }
  }
}