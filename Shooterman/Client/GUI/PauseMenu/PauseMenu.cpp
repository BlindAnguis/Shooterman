#include "PauseMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

PauseMenu::PauseMenu() {
    mName = "CLIENT: PAUSE_MENU";
    mDebugEnabled = true;

    mGuiFrame = std::make_shared<Frame>();

    auto pauseMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

    mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Pause"));

    pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Unpause", GAME_STATE::PREVIOUS));
    pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Leave Game", GAME_STATE::MAIN_MENU));
    pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Options", GAME_STATE::OPTIONS));

    pauseMenuList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Exit Game", []() {
      GameStateMessage gsm(GAME_STATE::MAIN_MENU);

      Subscriber gameStateSubscriber;
      MessageHandler::get().subscribeTo("ClientGameState", &gameStateSubscriber);
      gameStateSubscriber.reverseSendMessage(gsm.pack());
      MessageHandler::get().unsubscribeTo("ClientGameState", &gameStateSubscriber);

      sf::sleep(sf::milliseconds(100));
      sf::Packet shutdownMessage;
      shutdownMessage << SHUT_DOWN;
      MessageHandler::get().pushSystemMessage(shutdownMessage);
    }));

    mGuiFrame->addGuiComponent(pauseMenuList);
}

PauseMenu::~PauseMenu() { }