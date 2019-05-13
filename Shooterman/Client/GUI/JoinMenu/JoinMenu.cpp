#include "JoinMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiComponentFactory.h"

JoinMenu::JoinMenu() {
  mName = "CLIENT: JOIN_MENU";

  mGuiFrame = std::make_shared<Frame>();
  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Join Host"));

  auto joinMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

  auto ipList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  mIpText = std::make_shared<GuiText>(GuiComponentPosition::CENTER, "Enter IP");
  ipList->addGuiComponent(mIpText);
  ipList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, " Join", [this](){
    while (mPc.getNumberOfSubscribers() == 0) {
      sf::sleep(sf::milliseconds(5));
    }

    IpMessage ipm(mIpString, 1337);
    mPc.pushMessage(ipm.pack());

    GameStateMessage gsm(GAME_STATE::PLAYING);
    Subscriber gameStateSubscriber;
    MessageHandler::get().subscribeTo("GameState", &gameStateSubscriber);
    gameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("GameState", &gameStateSubscriber);
  }));

  joinMenuList->addGuiComponent(ipList);
  joinMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::MAIN_MENU));

  mGuiFrame->addGuiComponent(joinMenuList);
}

JoinMenu::~JoinMenu() { uninit(); }

void JoinMenu::init() {
  MessageHandler::get().publishInterface("ClientIpList", &mPc);
}

void JoinMenu::uninit() {
  sf::Packet shutdownRequest;
  shutdownRequest << 0;
  mPc.pushMessage(shutdownRequest);
  MessageHandler::get().unpublishInterface("ClientIpList");
}

void JoinMenu::handleNewText(sf::Uint32 newChar) {
  if (newChar == 8) {
    // Backspace
    mIpString = mIpString.substr(0, mIpString.size() - 1);
  } else if (mIpString.length() < 15) {
    mIpString += newChar;
  }
  mIpText->setText(mIpString);
}