#pragma once

#include "../Resources/GuiInputText.h"
#include "../Resources/MenuBase.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/Trace.h"

class JoinMenu : public MenuBase {
public:
  JoinMenu(std::shared_ptr<MessageHandler> messageHandler);
  ~JoinMenu();

  void init() override;
  void uninit() override;

  void backgroundUpdate() override;

private:
  std::string mIpString;
  std::shared_ptr<GuiInputText> mIpText;
  Interface mIpInterface;
  std::shared_ptr<MessageHandler> mMessageHandler;

  void onIpTextClicked();
  void onJoinClicked();
};