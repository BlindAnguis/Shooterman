#pragma once
#include "../Resources/MenuBase.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"
#include "../Resources/GuiList.h"

class DebugMenu : public MenuBase {
public:
  DebugMenu();
  ~DebugMenu();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  Interface mIf;
  std::shared_ptr<GuiList> mGuiList;
  int mCurrentYPos = 50;
};

