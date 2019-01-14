#pragma once
#include "../Resources/MenuBase.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"

class DebugMenu : public MenuBase {
public:
  DebugMenu();
  ~DebugMenu();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  Interface mIf;
  int mCurrentYPos = 50;
};

