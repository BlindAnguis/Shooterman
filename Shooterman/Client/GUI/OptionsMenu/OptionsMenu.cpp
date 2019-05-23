#include "OptionsMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiComponentFactory.h"

#include <SFML/Graphics.hpp>

OptionsMenu::OptionsMenu() {
  mName = "CLIENT: OPTIONS_MENU";
  mDebugEnabled = true;

  mGuiFrame = std::make_shared<Frame>();

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Options"));

  auto optionsMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);
  optionsMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::PREVIOUS));

  mGuiFrame->addGuiComponent(optionsMenuList);
}

OptionsMenu::~OptionsMenu() { }
