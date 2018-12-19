#pragma once

#include "../Resources/MenuBase.h"
#include "../../MessageHandler/PrivateCommunication.h"
#include "../../../Common/Trace.h"
#include "../../../Common/Messages/ClientInternal/IpMessage.h"

class JoinMenu : public MenuBase {
public:
  JoinMenu();
  ~JoinMenu();

private:
  PrivateCommunication mPc;
};