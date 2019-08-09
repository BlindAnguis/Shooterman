#include "JoinMenu.h"

#include "../Resources/GuiButton.h"
#include "../Resources/GuiList.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/Messages/ClientInternal/IpMessage.h"
#include "../../../Common/Interfaces.h"

JoinMenu::JoinMenu() {
  mName = "CLIENT: JOIN_MENU";

  mGuiFrame = std::make_shared<Frame>();
  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Join Host"));

  auto joinMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

  auto ipList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  mIpText = std::make_shared<GuiInputText>(GuiComponentPosition::CENTER, "Enter IP", std::bind(&JoinMenu::onIpTextClicked, this));
  mIpText->enableReceiveInput();
  addTextListener(mIpText);

  ipList->addGuiComponent(mIpText);
  ipList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, " Join", std::bind(&JoinMenu::onJoinClicked, this)));

  joinMenuList->addGuiComponent(ipList);
  joinMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::MAIN_MENU));

  mGuiFrame->addGuiComponent(joinMenuList);

  setupDebugMessages("Client", "Join Menu");
}

JoinMenu::~JoinMenu() { uninit(); }

void JoinMenu::init() {
  MessageHandler::get().publishInterface(Interfaces::CLIENT_IP_LIST, &mIpInterface);
  mIpString = "";
  mIpText->setText("");
}

void JoinMenu::uninit() {
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_IP_LIST);
}

void JoinMenu::backgroundUpdate() {
  handleDebugMessages();
}

void JoinMenu::onIpTextClicked() {
  while (mIpInterface.getNumberOfSubscribers() == 0) {
    sf::sleep(sf::milliseconds(5));
  }

  IpMessage ipm(mIpString, 1337);
  mIpInterface.pushMessage(ipm.pack());

  GameStateMessage gsm(GAME_STATE::CLIENT_LOBBY);
  Subscriber gameStateSubscriber;
  MessageHandler::get().subscribeTo("ClientGameState", &gameStateSubscriber);
  TRACE_SEND("GAME_STATE: CLIENT_LOBBY");
  gameStateSubscriber.reverseSendMessage(gsm.pack());
  MessageHandler::get().unsubscribeTo("ClientGameState", &gameStateSubscriber);
}

void JoinMenu::onJoinClicked() {
  while (mIpInterface.getNumberOfSubscribers() == 0) {
    sf::sleep(sf::milliseconds(5));
  }

  IpMessage ipm(mIpString, 1337);
  mIpInterface.pushMessage(ipm.pack());

  GameStateMessage gsm(GAME_STATE::CLIENT_LOBBY);
  Subscriber gameStateSubscriber;
  MessageHandler::get().subscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
  TRACE_SEND("GAME_STATE: CLIENT_LOBBY");
  gameStateSubscriber.reverseSendMessage(gsm.pack());
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
}
