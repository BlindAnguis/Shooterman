#pragma once

#include "../Resources/MenuBase.h"

class PauseMenu : public MenuBase {
public:
  PauseMenu();
  ~PauseMenu();

private:
  void onExitClick();
};