#include "GuiComponentFactory.h"

#include "GuiButton.h"
#include "../../../Common/MessageHandler/Interface.h"

std::shared_ptr<GuiComponent> GCF::createHeader(GuiComponentPosition guiComponentPosition, std::string text) {
  auto heading = std::make_shared<GuiText>(GuiComponentPosition::TOP, text);
  heading->setTextSize(72);
  return heading;
}

std::shared_ptr<GuiComponent> GCF::createGameStateButton(GuiComponentPosition guiComponentPosition, std::string text, GAME_STATE gameState) {
  auto gameStateButton = std::make_shared<GuiButton>(guiComponentPosition, text, [gameState] {
    GameStateMessage gsm(gameState);
    Subscriber gameStateSubscriber;
    MessageHandler::get().subscribeTo("GameState", &gameStateSubscriber);
    gameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("GameState", &gameStateSubscriber);
  });

  return gameStateButton;
}