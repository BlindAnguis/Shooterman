#include "JoinMenu.h"

JoinMenu::JoinMenu() {
  mName = "CLIENT: JOIN_MENU";

  MessageHandler::get().publishInterface("ClientIpList", &mPc);

  mComponentList.push_back(GUIComponentBuilder::createTitle("Join Host", 250, 36));
  auto ipButton = GUIComponentBuilder::createText("10.41.4.19", 100, 200);
  mComponentList.push_back(ipButton);
  mComponentList.push_back(GUIComponentBuilder::createCustomActionButton(" Join", 300, 200, [this, ipButton]() {
    while (mPc.getNumberOfSubscribers() == 0) {
      sf::sleep(sf::milliseconds(5));
    }

    IpMessage ipm(ipButton->getText(), 1337);
    mPc.pushMessage(ipm.pack());

    GameStateMessage gsm(GAME_STATE::PLAYING);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
    MessageHandler::get().unpublishInterface("ClientIpList");
  }));

  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Back", 250, 380, GAME_STATE::MAIN_MENU));
}

JoinMenu::~JoinMenu() { }
