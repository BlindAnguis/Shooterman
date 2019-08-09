#include "MainMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../Resources/GuiImageButton.h"
#include "../Resources/GuiComponentFactory.h"
#include "../Resources/GuiResourceManager.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

MainMenu::MainMenu() {
  mName = "CLIENT: MAIN_MENU";
  mDebugEnabled1 = true;

  mGuiFrame = std::make_shared<Frame>();

  auto mainMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Shooterman"));

  mainMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Host Game", GAME_STATE::LOBBY));
  mainMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Join Game", GAME_STATE::JOIN));
  mainMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Map Editor", GAME_STATE::MAP_EDITOR));
  mainMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Options", GAME_STATE::OPTIONS));

  mainMenuList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Exit Game", std::bind(&MainMenu::onExitCLick, this)));
  
  mGuiFrame->addGuiComponent(mainMenuList);

  setupDebugMessages("Client", "Main Menu");
}

MainMenu::~MainMenu() { }

void MainMenu::backgroundUpdate() {
  handleDebugMessages();
}

void MainMenu::onExitCLick() {
  sf::Packet shutdownMessage;
  shutdownMessage << MessageId::SHUT_DOWN;
  Subscriber s;
  MessageHandler::get().subscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
  TRACE_SEND("SHUT_DOWN");
  s.reverseSendMessage(shutdownMessage);
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
}
