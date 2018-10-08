#pragma once

#include "../Resources/MenuBase.h"
#include "../../../Common/Trace.h"

class JoinMenu : public MenuBase {
public:
  JoinMenu();
  ~JoinMenu();

  static void changeGameStateToMainMenu();
};