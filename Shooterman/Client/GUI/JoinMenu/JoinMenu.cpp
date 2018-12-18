#include "JoinMenu.h"

JoinMenu::JoinMenu() {
  mName = "CLIENT: JOIN_MENU";

  loadFont("RobbieRocketpants.ttf");

  sf::Color textColor = sf::Color::Black;
  sf::Color textHighlightColor = sf::Color(0, 120, 0);

  mButtonList.push_back(std::make_shared<GUIButton>("Join host", mFont, textColor, textColor, 72, 250, 36));

  auto ipButton = std::make_shared<GUIButton>("10.41.4.19", mFont, textColor, textColor, 56, 100, 200);
  mButtonList.push_back(ipButton);

  MessageHandler::get().publishInterface("ClientIpList", &mPc);
  std::function<void()> joinCallback([this, ipButton]() {

    while (mPc.getNumberOfSubscribers() == 0) {
      sf::sleep(sf::milliseconds(5));
    }
    
    IpMessage ipm(ipButton->getText(), 1337);
    mPc.pushMessage(ipm.pack());

    GameStateMessage gsm(GAME_STATE::PLAYING);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
    MessageHandler::get().unpublishInterface("ClientIpList");
  });

  auto b = std::make_shared<GUIButton>(" Join", mFont, textColor, textHighlightColor, 56, 300, 200);
  b->setCallback(joinCallback);
  mButtonList.push_back(b);


  std::function<void()> mainMenuCallback([]() {
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
  });
  auto mainMenuButton = std::make_shared<GUIButton>("Back", mFont, textColor, textHighlightColor, 56, 250, 380, &changeGameStateToMainMenu);
  mainMenuButton->setCallback(mainMenuCallback);
  mButtonList.push_back(mainMenuButton);
}

JoinMenu::~JoinMenu() { }

void JoinMenu::changeGameStateToMainMenu() {
}