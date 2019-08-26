#pragma once

#include "GuiButton.h"
#include "GuiComponent.h"
#include "../../../Common/Messages/GameStateMessage.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class GCF {
public:
  static std::shared_ptr<GuiComponent> createHeader(GuiComponentPosition guiComponentPosition, std::string text);
  static std::shared_ptr<GuiButton> createGameStateButton(GuiComponentPosition guiComponentPosition, std::string text, GAME_STATE gameState, std::shared_ptr<MessageHandler> mMessageHandler, bool disabled = false);
  static std::shared_ptr<GuiText> createTextBox(GuiComponentPosition guiComponentPosition, std::string text);
};
