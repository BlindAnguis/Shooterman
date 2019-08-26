#include "GuiComponentFactory.h"

#include "GuiButton.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/Interfaces.h"

std::shared_ptr<GuiComponent> GCF::createHeader(GuiComponentPosition guiComponentPosition, std::string text) {
  auto heading = std::make_shared<GuiText>(GuiComponentPosition::TOP, text, 72);
  return heading;
}

std::shared_ptr<GuiButton> GCF::createGameStateButton(GuiComponentPosition guiComponentPosition, std::string text, GAME_STATE gameState, std::shared_ptr<MessageHandler> messageHandler, bool disabled) {
  auto gameStateButton = std::make_shared<GuiButton>(guiComponentPosition, text, [gameState, messageHandler] {
    GameStateMessage gsm(gameState);
    Subscriber gameStateSubscriber;
    messageHandler->subscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
    gameStateSubscriber.reverseSendMessage(gsm.pack());
    messageHandler->unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
  });
  if (disabled) {
    gameStateButton->setDisabled();
  }
  return gameStateButton;
}


std::shared_ptr<GuiText> GCF::createTextBox(GuiComponentPosition guiComponentPosition, std::string text) {
  auto heading = std::make_shared<GuiText>(guiComponentPosition, text, 42);
  heading->setOutlineColor(sf::Color::Red);
  return heading;
}