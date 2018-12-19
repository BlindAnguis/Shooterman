#pragma once

#include "../Resources/MenuBase.h"
#include "../../MessageHandler/PrivateCommunication.h"
#include "../../../Common/Trace.h"
#include "../../../Common/Messages/ClientInternal/IpMessage.h"

class JoinMenu : public MenuBase {
public:
  JoinMenu();
  ~JoinMenu();

  void handleNewText(sf::Uint32 newChar) override;

private:
  std::string mIpString;
  std::shared_ptr<GUIComponent> mIPInputText;
  PrivateCommunication mPc;
};