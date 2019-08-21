#pragma once

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1 // Remove stupid google test warning, or something

#include "gtest/gtest.h"

#include "../Shooterman/Common/MessageHandler/Interface.cpp"

class NetworkHandlerTest : public ::testing::Test {
public:
  NetworkHandlerTest();
  ~NetworkHandlerTest();

  void SetUp();
  void TearDown();

  Interface mGameStateInterface;
  Interface mInfoInterface;
  Interface mInputInterface;
  Interface mDebugInterface;

  void setupInterfaces();
  void teardownInterfaces();
};

