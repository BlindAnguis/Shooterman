#include "PauseMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiButton.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"

PauseMenu::PauseMenu(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler) {
  mName = "CLIENT: PAUSE_MENU";
  mDebugEnabled1 = true;

  mGuiFrame = std::make_shared<Frame>();

  auto pauseMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Pause"));

  pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Unpause", GAME_STATE::PREVIOUS, mMessageHandler));
  pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Leave Game", GAME_STATE::MAIN_MENU, mMessageHandler));
  pauseMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Options", GAME_STATE::OPTIONS, mMessageHandler));

  pauseMenuList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Exit Game", std::bind(&PauseMenu::onExitClick, this)));

  mGuiFrame->addGuiComponent(pauseMenuList);

  setupDebugMessages("Client", "Pause Menu", mMessageHandler);
}

PauseMenu::~PauseMenu() { }

void PauseMenu::backgroundUpdate() {
  handleDebugMessages();
}

void PauseMenu::onExitClick() {
  GameStateMessage gsm(GAME_STATE::MAIN_MENU);

  Subscriber gameStateSubscriber;
  mMessageHandler->subscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
  TRACE_SEND("GAME_STATE: MAIN_MENU");
  gameStateSubscriber.reverseSendMessage(gsm.pack());
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);

  sf::sleep(sf::milliseconds(100));
  sf::Packet shutdownMessage;
  shutdownMessage << MessageId::SHUT_DOWN;
  Subscriber s;
  mMessageHandler->subscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
  TRACE_SEND("SHUT_DOWN");
  s.reverseSendMessage(shutdownMessage);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
}
