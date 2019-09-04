#pragma once

#include "gtest/gtest.h"

#include "../Shooterman/Client/Input/Input.cpp"
#include "../Shooterman/Client/Input/Mock/KeyboardMock.cpp"
#include "../Shooterman/Common/MessageHandler/Mock/MessageHandlerMock.cpp"

class InputTest : public ::testing::Test {
public:
  InputTest();
  ~InputTest();

  void SetUp();
  void TearDown();

protected:
  std::shared_ptr<Input> mInput;
  std::shared_ptr<MessageHandlerMock> mMessageHandlerMock;
  std::shared_ptr<KeyboardMock> mKeyboardMock;
};

