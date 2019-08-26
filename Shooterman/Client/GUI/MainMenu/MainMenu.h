#pragma once

#include "../Resources/MenuBase.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class MainMenu : public MenuBase {
public:
  MainMenu(std::shared_ptr<MessageHandler> messageHandler);
  ~MainMenu();

  void backgroundUpdate() override;

private:
  std::shared_ptr<MessageHandler> mMessageHandler;

  void onExitCLick();
};