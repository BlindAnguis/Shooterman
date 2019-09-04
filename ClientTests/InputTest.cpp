#include "InputTest.h"

#include "../Shooterman/Common/Messages/MouseMessage.cpp"
#include "../Shooterman/Common/Messages/InputMessage.cpp"

InputTest::InputTest() {}


InputTest::~InputTest() {}

void InputTest::SetUp() {
  mMessageHandlerMock = std::make_shared<MessageHandlerMock>();
  mKeyboardMock = std::make_shared<KeyboardMock>();
  mInput = std::make_shared<Input>(mMessageHandlerMock, mKeyboardMock);

  mInput->start();
}

void InputTest::TearDown() {
  mInput->stop();

  mInput = nullptr;
  mMessageHandlerMock = nullptr;
  mKeyboardMock = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler subscribes/unsubscribes to all interfaces and 
// publishes/unpublishes all interfaces
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(InputTest, verifyBasicStartupAndShutdown) {
  mInput->run();
}