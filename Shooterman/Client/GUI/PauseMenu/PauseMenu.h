#pragma once

#include "../Resources/MenuBase.h"

class PauseMenu : public MenuBase {
public:
  PauseMenu();
  ~PauseMenu();

  void backgroundUpdate() override;

private:
  void onExitClick();
};