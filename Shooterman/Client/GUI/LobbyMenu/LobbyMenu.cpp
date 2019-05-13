#include "LobbyMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiComponentFactory.h"

LobbyMenu::LobbyMenu() {
  mName = "CLIENT: LOBBY_MENU";
  mDebugEnabled = true;

  mGuiFrame = std::make_shared<Frame>();

  auto lobbyMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Join Lobby"));

  lobbyMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Start Game", GAME_STATE::PLAYING));
  lobbyMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::MAIN_MENU));

  mGuiFrame->addGuiComponent(lobbyMenuList);
}

LobbyMenu::~LobbyMenu() { }