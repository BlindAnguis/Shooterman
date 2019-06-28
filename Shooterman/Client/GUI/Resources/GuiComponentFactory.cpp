#include "GuiComponentFactory.h"

#include "GuiButton.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/Interfaces.h"

std::shared_ptr<GuiComponent> GCF::createHeader(GuiComponentPosition guiComponentPosition, std::string text) {
  auto heading = std::make_shared<GuiText>(GuiComponentPosition::TOP, text, 72);
  return heading;
}

std::shared_ptr<GuiButton> GCF::createGameStateButton(GuiComponentPosition guiComponentPosition, std::string text, GAME_STATE gameState, bool disabled) {
  auto gameStateButton = std::make_shared<GuiButton>(guiComponentPosition, text, [gameState] {
    GameStateMessage gsm(gameState);
    Subscriber gameStateSubscriber;
    MessageHandler::get().subscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
    gameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &gameStateSubscriber);
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