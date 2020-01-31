#include "NetworkHandlerTest.h"

#include "../Shooterman/Common/Trace.cpp"
#include "../Shooterman/Common/Process/Runnable.cpp"
#include "../Shooterman/Common/MessageHandler/Subscriber.cpp"
#include "../Shooterman/Common/MessageHandler/Interface.cpp"
#include "../Shooterman/Common/MessageHandler/MessageHandlerImpl.cpp"
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
#include "../Shooterman/Common/Messages/ScoreBoardMessage.cpp"
#include "../Shooterman/Common/Network/SocketImpl.cpp"
#include "../Shooterman/Common/Constants.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

NetworkHandlerTest::NetworkHandlerTest() {}

NetworkHandlerTest::~NetworkHandlerTest() {}

void NetworkHandlerTest::SetUp() {
  mMessageHandlerMock = std::make_shared<MessageHandlerMock>();
  mSocketMock = std::make_shared<SocketMock>();
  mNetworkHandler = std::make_shared<NetworkHandler>(mMessageHandlerMock, std::make_shared<SocketFactoryMocker>(mSocketMock));

  expectSubscribersToSubscribe();
  expectInterfacesToPublish();
  mNetworkHandler->start();
}

void NetworkHandlerTest::TearDown() {
  expectSubscribersToUnsubscribe();
  expectInterfacesToUnpublish();

  mNetworkHandler->stop();

  mSubscriberMap.clear();
  mNetworkHandler = nullptr;
  mMessageHandlerMock = nullptr;
}

void NetworkHandlerTest::expectSubscribeTo(std::string interfaceName) {
  EXPECT_CALL(*mMessageHandlerMock, subscribeToWithTimeoutImpl(interfaceName, _, _))
    .WillOnce(Invoke([this](std::string interfaceName, Subscriber* subscriber, int dontCare) {
    mSubscriberMap.insert(std::make_pair(interfaceName, subscriber));
  }));
}

void NetworkHandlerTest::expectSubscribe(std::string interfaceName) {
  EXPECT_CALL(*mMessageHandlerMock, subscribeTo(interfaceName, _))
    .WillOnce(Invoke([this](std::string interfaceName, Subscriber* subscriber) {
    mSubscriberMap.insert(std::make_pair(interfaceName, subscriber));
    return true;
  }));
}

void NetworkHandlerTest::expectSubscribersToSubscribe() {
  expectSubscribeTo(Interfaces::CLIENT_GAME_STATE);
  expectSubscribeTo(Interfaces::INFO_MESSAGE);
  expectSubscribeTo(Interfaces::CLIENT_DEBUG_MENU);
  expectSubscribe(Interfaces::CLIENT_INPUT_LIST);
  expectSubscribe(Interfaces::CLIENT_DEBUG_MENU);
  expectSubscribe(Interfaces::CLIENT_GAME_STATE);
}

void NetworkHandlerTest::expectInterfacesToPublish() {
  EXPECT_CALL(*mMessageHandlerMock, publishInterface(Interfaces::CLIENT_SPRITE_LIST, _)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, publishInterface(Interfaces::CLIENT_PLAYER_DATA, _)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, publishInterface(Interfaces::CLIENT_SOUND_LIST, _)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, publishInterface(Interfaces::CLIENT_LOBBY, _)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, publishInterface(Interfaces::CLIENT_SERVER_READY, _)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, publishInterface(Interfaces::CLIENT_SCORE_BOARD, _)).Times(1);
}

void NetworkHandlerTest::expectSubscribersToUnsubscribe() {
  EXPECT_CALL(*mMessageHandlerMock, unsubscribeTo(Interfaces::CLIENT_GAME_STATE, _)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, unsubscribeTo(Interfaces::INFO_MESSAGE, _)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, unsubscribeTo(Interfaces::CLIENT_INPUT_LIST, _)).Times(1);
  // 1 time for the networkHandler and 1 time for the server (via socket)
  EXPECT_CALL(*mMessageHandlerMock, unsubscribeTo(Interfaces::CLIENT_DEBUG_MENU, _)).Times(2);
}

void NetworkHandlerTest::expectInterfacesToUnpublish() {
  EXPECT_CALL(*mMessageHandlerMock, unpublishInterface(Interfaces::CLIENT_SPRITE_LIST)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, unpublishInterface(Interfaces::CLIENT_PLAYER_DATA)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, unpublishInterface(Interfaces::CLIENT_SOUND_LIST)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, unpublishInterface(Interfaces::CLIENT_LOBBY)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, unpublishInterface(Interfaces::CLIENT_SERVER_READY)).Times(1);
  EXPECT_CALL(*mMessageHandlerMock, unpublishInterface(Interfaces::CLIENT_SCORE_BOARD)).Times(1);
}

void NetworkHandlerTest::sendGameStateLobby(bool expectSubscribe) {
  if (expectSubscribe) {
    // Expect network handler to subscribe to ip messages
    expectSubscribeTo(Interfaces::CLIENT_IP_LIST);
  }
  GameStateMessage gsmL(GAME_STATE::LOBBY);
  mSubscriberMap[Interfaces::CLIENT_GAME_STATE]->sendMessage(gsmL.pack());

  mNetworkHandler->handleSubscribers();
  mNetworkHandler->run();
}

void NetworkHandlerTest::sendGameStateJoin(bool expectSubscribe) {
  if (expectSubscribe) {
    // Expect network handler to subscribe to ip messages
    expectSubscribeTo(Interfaces::CLIENT_IP_LIST);
  }
  GameStateMessage gsmL(GAME_STATE::JOIN);
  mSubscriberMap[Interfaces::CLIENT_GAME_STATE]->sendMessage(gsmL.pack());

  mNetworkHandler->handleSubscribers();
  mNetworkHandler->run();
}

void NetworkHandlerTest::sendGameStateMainMenu(bool hasSocketBeenConnected) {
  // Expect network handler to disconnect socket
  EXPECT_CALL(*mMessageHandlerMock, unsubscribeTo(Interfaces::CLIENT_IP_LIST, _)).Times(1);
  GameStateMessage gsmM(GAME_STATE::MAIN_MENU);
  mSubscriberMap[Interfaces::CLIENT_GAME_STATE]->sendMessage(gsmM.pack());

  EXPECT_CALL(*mSocketMock, disconnect());
  mNetworkHandler->handleSubscribers();
  mNetworkHandler->run();
}

void NetworkHandlerTest::sendIpMessage(Soc::Status connectionResult) {
  // Expect network handler to connect to server socket after receiving a ip message
  IpMessage im("localhost", IP_PORT);
  mSubscriberMap[Interfaces::CLIENT_IP_LIST]->sendMessage(im.pack());
  
  EXPECT_CALL(*mSocketMock, connect("localhost", IP_PORT, _)).WillOnce(Return(connectionResult));
  mNetworkHandler->handleSubscribers();
  mNetworkHandler->run();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler subscribes/unsubscribes to all interfaces and 
// publishes/unpublishes all interfaces
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifyBasicStartupAndShutdown) {
  mNetworkHandler->run();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler connets to a server socket after receiving the game state LOBBY
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifySocketConnectByLobbyState) {
  sendGameStateLobby();
  sendIpMessage();
  sendGameStateMainMenu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler connets to a server socket after receiving the game state JOIN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifySocketConnectByJoinState) {
  sendGameStateJoin();
  sendIpMessage();
  sendGameStateMainMenu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler only subscribes to ip messages once when receiving both game 
// state LOBBY and JOIN (should not happen in normal case)
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifySocketConnectByLobbyJoinState) {
  sendGameStateLobby();
  sendGameStateJoin(false);
  sendIpMessage();
  sendGameStateMainMenu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler subscribes again after going back to main menu
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifySubscribeToIpTwice) {
  sendGameStateLobby();
  sendGameStateMainMenu(false);
  sendGameStateLobby();
  sendIpMessage();
  sendGameStateMainMenu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Verify that the network handler subscribes again after going back to main menu
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_F(NetworkHandlerTest, verifyGoesBackToCorrectStateAfterFailedSocketConnection) {
  sendGameStateLobby();

  mSubscriberMap[Interfaces::CLIENT_GAME_STATE]->setCallback([](sf::Packet packet) {
    int id = -1;
    packet >> id;
    ASSERT_EQ(id, MessageId::CHANGE_GAME_STATE);
    GameStateMessage gsm(packet);
    ASSERT_EQ(gsm.getGameState(), GAME_STATE::PREVIOUS);
  });

  sendIpMessage(Soc::Status::NotReady);

  for (int i = 0; i < 10; ++i) {
    EXPECT_CALL(*mSocketMock, connect("localhost", IP_PORT, _)).WillOnce(Return(Soc::Status::NotReady));
    mNetworkHandler->run();
  }
}