#include "MainMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/MessageId.h"

MainMenu::MainMenu() {
  mName = "CLIENT: MAIN_MENU";
  mDebugEnabled = true;

  mGuiFrame = std::make_shared<Frame>();

  auto mainMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Shooterman"));

  mainMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Host Game", GAME_STATE::LOBBY));
  mainMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Join Game", GAME_STATE::JOIN));
  mainMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Options", GAME_STATE::OPTIONS));

  mainMenuList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Exit Game", []() {
    sf::Packet shutdownMessage;
    shutdownMessage << SHUT_DOWN;
    MessageHandler::get().pushSystemMessage(shutdownMessage);
  }));

  mGuiFrame->addGuiComponent(mainMenuList);
}

MainMenu::~MainMenu() { }