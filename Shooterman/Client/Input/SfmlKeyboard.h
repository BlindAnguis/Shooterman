#pragma once

#include "Keyboard.h"

class SfmlKeyboard : public Keyboard {
public:
  SfmlKeyboard();
  ~SfmlKeyboard();

  bool isKeyPressed(KEY key) override;
};

