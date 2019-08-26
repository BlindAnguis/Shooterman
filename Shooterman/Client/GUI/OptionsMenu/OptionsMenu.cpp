#include "OptionsMenu.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiComponentFactory.h"

OptionsMenu::OptionsMenu(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler) {
  mName = "CLIENT: OPTIONS_MENU";
  mDebugEnabled1 = true;

  mGuiFrame = std::make_shared<Frame>();

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Options"));

  auto optionsMenuList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);
  optionsMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::PREVIOUS, mMessageHandler));

  mGuiFrame->addGuiComponent(optionsMenuList);
}

OptionsMenu::~OptionsMenu() { }
