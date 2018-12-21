#include "JoinMenu.h"

JoinMenu::JoinMenu() {
  mName = "CLIENT: JOIN_MENU";


  mComponentList.push_back(GUIComponentBuilder::createTitle("Join Host", 250, 36));
  mIPInputText = GUIComponentBuilder::createText("Enter IP", 100, 200);
  mComponentList.push_back(mIPInputText);
  mComponentList.push_back(GUIComponentBuilder::createCustomActionButton(" Join", 300, 200, [this]() {
    while (mPc.getNumberOfSubscribers() == 0) {
      sf::sleep(sf::milliseconds(5));
    }

    IpMessage ipm(mIpString, 1337);
    mPc.pushMessage(ipm.pack());

    GameStateMessage gsm(GAME_STATE::PLAYING);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
  }));

  mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Back", 250, 380, GAME_STATE::MAIN_MENU));
}

JoinMenu::~JoinMenu() { }

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
  mIPInputText->setText(mIpString);
}