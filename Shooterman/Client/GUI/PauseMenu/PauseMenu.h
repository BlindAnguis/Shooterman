#pragma once

#include <memory>

#include "../Resources/MenuBase.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class PauseMenu : public MenuBase {
public:
  PauseMenu(std::shared_ptr<MessageHandler> messageHandler);
  ~PauseMenu();

  void backgroundUpdate() override;

private:
  std::shared_ptr<MessageHandler> mMessageHandler;

  void onExitClick();
};