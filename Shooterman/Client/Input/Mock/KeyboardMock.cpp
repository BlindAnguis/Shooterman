#pragma once
#include "gmock/gmock.h"

#include "../Keyboard.h"

class KeyboardMock : public Keyboard {
public:
  MOCK_METHOD1(isKeyPressed, bool(KEY key));
};