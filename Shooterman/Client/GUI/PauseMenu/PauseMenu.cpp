#include "PauseMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

PauseMenu::PauseMenu() {
    mName = "CLIENT: PAUSE_MENU";
    mDebugEnabled1 = true;

    mGuiFrame = std::make_shared<Frame>();

    auto pauseMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

    mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Pause"));

    pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Unpause", GAME_STATE::PREVIOUS));
    pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Leave Game", GAME_STATE::MAIN_MENU));
    pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Options", GAME_STATE::OPTIONS));

    pauseMenuList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Exit Game", std::bind(&PauseMenu::onExitClick, this)));

    mGuiFrame->addGuiComponent(pauseMenuList);
}

PauseMenu::~PauseMenu() { }

void PauseMenu::onExitClick() {
  GameStateMessage gsm(GAME_STATE::MAIN_MENU);

  Subscriber gameStateSubscriber;
  MessageHandler::get().subscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
  TRACE_SEND("GAME_STATE: MAIN_MENU");
  gameStateSubscriber.reverseSendMessage(gsm.pack());
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);

  sf::sleep(sf::milliseconds(100));
  sf::Packet shutdownMessage;
  shutdownMessage << MessageId::SHUT_DOWN;
  Subscriber s;
  MessageHandler::get().subscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
  TRACE_SEND("SHUT_DOWN");
  s.reverseSendMessage(shutdownMessage);
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
}
