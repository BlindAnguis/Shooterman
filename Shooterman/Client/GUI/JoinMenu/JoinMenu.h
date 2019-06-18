#pragma once

#include "../Resources/GuiInputText.h"
#include "../Resources/MenuBase.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/Trace.h"

class JoinMenu : public MenuBase {
public:
  JoinMenu();
  ~JoinMenu();

  void init() override;
  void uninit() override;

  void handleNewText(sf::Uint32 newChar) override;

private:
  std::string mIpString;
  std::shared_ptr<GuiInputText> mIpText;
  Interface mIpInterface;
};