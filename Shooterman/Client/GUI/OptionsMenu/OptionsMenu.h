#pragma once
#include "../Resources/MenuBase.h"

class OptionsMenu : public MenuBase
{
public:
  OptionsMenu();
  ~OptionsMenu();

  static void changeGameStateToMainMenu();
};

