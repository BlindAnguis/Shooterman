#pragma once

#include "GuiComponent.h"
#include "../../../Common/Messages/GameStateMessage.h"
#include "GuiButton.h"

class GCF {
public:
  static std::shared_ptr<GuiComponent> createHeader(GuiComponentPosition guiComponentPosition, std::string text);
  static std::shared_ptr<GuiButton> createGameStateButton(GuiComponentPosition guiComponentPosition, std::string text, GAME_STATE gameState, bool disabled = false);
};