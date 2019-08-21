#include "NetworkHandlerTest.h"

#include"../Shooterman/Client/Network/NetworkHandler.cpp"

#include "../Shooterman/Common/Trace.cpp"
#include "../Shooterman/Common/MessageHandler/Subscriber.cpp"
#include "../Shooterman/Common/MessageHandler/MessageHandler.cpp"
#include "../Shooterman/Common/Messages/AddDebugButtonMessage.cpp"
#include "../Shooterman/Common/Messages/RemoveDebugButtonMessage.cpp"
#include "../Shooterman/Common/Messages/ClientMainAndNetworkHandlerMessages.cpp"
#include "../Shooterman/Common/Messages/GameStateMessage.cpp"
#include "../Shooterman/Common/Messages/LobbyDataMessage.cpp"
#include "../Shooterman/Common/Messages/PlayableCharactersMessage.cpp"
#include "../Shooterman/Common/Messages/ServerReadyMessage.cpp"
#include "../Shooterman/Common/Messages/PlayerDataMessage.cpp"
#include "../Shooterman/Common/Messages/SpriteMessage.cpp"
#include "../Shooterman/Common/Messages/SpriteCacheMessage.cpp"
#include "../Shooterman/Common/Messages/ClientInternal/IpMessage.cpp"
#include "../Shooterman/Common/Messages/SoundMessage.cpp"
#include "../Shooterman/Common/Messages/InfoMessage.cpp"
#include "../Shooterman/Common/Messages/MapMessage.cpp"
#include "../Shooterman/Common/Messages/ToggleDebugButtonMessage.cpp"
#include "../Shooterman/Common/Messages/SubscribeDoneMessage.cpp"
#include "../Shooterman/Common/Messages/SubscribeTimeoutMessage.cpp"

NetworkHandlerTest::NetworkHandlerTest() {}

NetworkHandlerTest::~NetworkHandlerTest() {}

void NetworkHandlerTest::SetUp() {
  setupInterfaces();
}

void NetworkHandlerTest::TearDown() {
  teardownInterfaces();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Publish all the interafaces used by network handler
///////////////////////////////////////////////////////////////////////////////////////////////////
void NetworkHandlerTest::setupInterfaces() {
  MessageHandler::get().publishInterface(Interfaces::CLIENT_GAME_STATE, &mGameStateInterface);
  MessageHandler::get().publishInterface(Interfaces::INFO_MESSAGE, &mInfoInterface);
  MessageHandler::get().publishInterface(Interfaces::CLIENT_INPUT_LIST, &mInputInterface);
  MessageHandler::get().publishInterface(Interfaces::CLIENT_DEBUG_MENU, &mDebugInterface);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Unpublish all the interafaces used by network handler
///////////////////////////////////////////////////////////////////////////////////////////////////
void NetworkHandlerTest::teardownInterfaces() {
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_GAME_STATE);
  MessageHandler::get().unpublishInterface(Interfaces::INFO_MESSAGE);
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_INPUT_LIST);
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_DEBUG_MENU);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler subscribes/unsubscribes to all interfaces
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifySubscribers) {
  NetworkHandler networkHandler;

  sf::sleep(sf::milliseconds(10)); // wait for messagehandler to handle new subscribers

  ASSERT_EQ(mGameStateInterface.getNumberOfSubscribers(), 2);
  ASSERT_EQ(mInfoInterface.getNumberOfSubscribers(), 1);
  ASSERT_EQ(mInputInterface.getNumberOfSubscribers(), 1);
  ASSERT_EQ(mDebugInterface.getNumberOfSubscribers(), 2);

  networkHandler.shutDown();

  ASSERT_EQ(mGameStateInterface.getNumberOfSubscribers(), 0);
  ASSERT_EQ(mInfoInterface.getNumberOfSubscribers(), 0);
  ASSERT_EQ(mInputInterface.getNumberOfSubscribers(), 0);
  ASSERT_EQ(mDebugInterface.getNumberOfSubscribers(), 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler publishes/unpublishes all interfaces
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifyInterfaces) {
  NetworkHandler networkHandler;

  sf::sleep(sf::milliseconds(10)); // wait for messagehandler to handle new subscribers



  networkHandler.shutDown();
}