#pragma once

#include "../Resources/MenuBase.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class OptionsMenu : public MenuBase {
public:
  OptionsMenu(std::shared_ptr<MessageHandler> messageHandler);
  ~OptionsMenu();

private:
  std::shared_ptr<MessageHandler> mMessageHandler;
};

