#include "NetworkHandler.h"

#include "../../Common/KeyBindings.h"
#include "../../Common/Constants.h"
#include "../../Common/MessageId.h"
#include "../../Common/Interfaces.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../Common/Messages/ClientMainAndNetworkHandlerMessages.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/Messages/LobbyDataMessage.h"
#include "../../Common/Messages/PlayableCharactersMessage.h"
#include "../../Common/Messages/ServerReadyMessage.h"
#include "../../Common/Messages/PlayerDataMessage.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/SpriteCacheMessage.h"
#include "../../Common/Messages/ClientInternal/IpMessage.h"
#include "../../Common/Messages/SoundMessage.h"
#include "../../Common/Messages/InfoMessage.h"
#include "../../Common/Messages/MapMessage.h"

NetworkHandler::NetworkHandler(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<SocketFactory> socketFactory) : 
  mMessageHandler(messageHandler),
  mSocketFactory(socketFactory) {
  mName = "CLIENT: NETWORK_HANDLER";
}

NetworkHandler::~NetworkHandler() { }

void NetworkHandler::start() {
  TRACE_INFO("Starting module...");

  mSocket = mSocketFactory->createTcpSocket();

  mIpSubscriber.addSignalCallback(MessageId::SUBSCRIBE_TIMEOUT, std::bind(&NetworkHandler::handleSubscribeIpListTimeoutMessage, this, std::placeholders::_1));
  mIpSubscriber.addSignalCallback(MessageId::IP_MESSAGE, std::bind(&NetworkHandler::handleIpListMessage, this, std::placeholders::_1));
  mGameStateSubscriber.addSignalCallback(MessageId::SUBSCRIBE_TIMEOUT, std::bind(&NetworkHandler::handleSubscribeGameStateTimeoutMessage, this, std::placeholders::_1));
  mGameStateSubscriber.addSignalCallback(MessageId::CHANGE_GAME_STATE, std::bind(&NetworkHandler::handleChangeGameStateMessage, this, std::placeholders::_1));

  startListenToSubscriber(&mIpSubscriber);
  startListenToSubscriber(&mGameStateSubscriber);

  mCurrentState = STATE::Disconnected;

  setupSubscribersAndInterfaces();

  mConnectionTriesLeft = 10;
  TRACE_INFO("Starting module done");
}

void NetworkHandler::run() {
  handleDebugMessages();

  switch (mCurrentState) {
  case STATE::Disconnected:
    mConnectionTriesLeft = 10;
    break;
  case STATE::Connecting: {
    Soc::Status result = mSocket->connect(mServerIp, mServerPort, 100);
    if (result == Soc::Status::Done) {
      TRACE_INFO("Connected!");
      mSocket->setBlocking(false);
      mHeartbeatClock.restart();
      mCurrentState = STATE::Connected;
    } else {
      sf::sleep(sf::milliseconds(200));
      --mConnectionTriesLeft;
      if (mConnectionTriesLeft < 0) {
        mCurrentState = STATE::Disconnected;
        TRACE_ERROR("Connection failed! " << result);
        InfoMessage msg("Connection failed.", 3);
        mInfoMessageSubscriber.reverseSendMessage(msg.pack());

        GameStateMessage gsm(GAME_STATE::JOIN);
        mGameStateSubscriber.reverseSendMessage(gsm.pack());
      }
    }
    break;
  }
  case STATE::Connected:
    handlePackets();
    break;
  case STATE::Disconnecting:
    mSocket->disconnect();
    if (mSubscribedToIpMessage) {
      mMessageHandler->unsubscribeTo(Interfaces::CLIENT_IP_LIST, &mIpSubscriber);
      mSubscribedToIpMessage = false;
    }
    mCurrentState = STATE::Disconnected;
    break;
  default:
    break;
  }

  sf::sleep(sf::milliseconds(1));
}

void NetworkHandler::stop() {
  TRACE_INFO("Stopping module...");
  teardownSubscribersAndInterfaces();

  stopListenToSubscriber(&mIpSubscriber);
  stopListenToSubscriber(&mGameStateSubscriber);
  TRACE_INFO("Stopping module done");
}

void NetworkHandler::setupSubscribersAndInterfaces() {
  setupDebugMessages("Client", "Network", mMessageHandler);

  mMessageHandler->publishInterface(Interfaces::CLIENT_SPRITE_LIST, &mSpriteListInterface);
  mMessageHandler->publishInterface(Interfaces::CLIENT_PLAYER_DATA, &mPlayerDataInterface);
  mMessageHandler->publishInterface(Interfaces::CLIENT_SOUND_LIST, &mSoundListInterface);
  mMessageHandler->publishInterface(Interfaces::CLIENT_LOBBY, &mLobbyInterface);
  mMessageHandler->publishInterface(Interfaces::CLIENT_SERVER_READY, &mServerReadyInterface);

  mMessageHandler->subscribeToWithTimeout(Interfaces::CLIENT_GAME_STATE, &mGameStateSubscriber);
  mMessageHandler->subscribeToWithTimeout(Interfaces::INFO_MESSAGE, &mInfoMessageSubscriber);

  mMessageHandler->subscribeTo(Interfaces::CLIENT_INPUT_LIST, &mForwaringSubscriber);
  mMessageHandler->subscribeTo(Interfaces::CLIENT_DEBUG_MENU, &mForwaringSubscriber);
  mMessageHandler->subscribeTo(Interfaces::CLIENT_GAME_STATE, &mForwaringSubscriber);
}

void NetworkHandler::handlePackets() {
  auto forwardMessageQueue = mForwaringSubscriber.getMessageQueue();
  while (!forwardMessageQueue.empty()) {
    sf::Packet packet = forwardMessageQueue.front();
    forwardMessageQueue.pop();
    mSocket->send(packet);
  }

  auto lobbyMessageQueue = mLobbyInterface.getMessageQueue();
  while (!lobbyMessageQueue.empty()) {
    sf::Packet packet = lobbyMessageQueue.front();
    lobbyMessageQueue.pop();
    mSocket->send(packet);
  }

  auto debugMessageQueue = mServerDebugSubscriber.getMessageQueue();
  while (!debugMessageQueue.empty()) {
    sf::Packet packet = debugMessageQueue.front();
    debugMessageQueue.pop();
    int id = -1;
    packet >> id;

    AddDebugButtonMessage adbm(packet);
    sf::Packet networkPacket = adbm.pack();
    mSocket->send(networkPacket);
  }

  sf::Packet packet;
  if (mSocket->receive(packet) == Soc::Status::Done) {
    int id = -1;
    packet >> id;
    if (id == MessageId::SPRITE_LIST_CACHE) {
      SpriteCacheMessage sm(packet);
      mSpriteListInterface.pushMessage(sm.pack());
    } else if (id == MessageId::SPRITE_LIST) {
      SpriteMessage sm(packet);
      mSpriteListInterface.pushMessage(sm.pack());
    } else if (id == MessageId::PLAYER_DATA) {
      PlayerDataMessage pdm(packet);
      mPlayerDataInterface.pushMessage(pdm.pack());
    } else if (id == MessageId::PLAYER_USERNAMES) {
      LobbyDataMessage ldm(packet);
      mLobbyInterface.pushMessage(ldm.pack());
    } else if (id == MessageId::PLAYABLE_CHARACTERS) {
      PlayableCharactersMessage pcm(packet);
      mLobbyInterface.pushMessage(pcm.pack());
    } else if (id == MessageId::CHANGE_GAME_STATE) {
      GameStateMessage gsm(packet);
      mGameStateSubscriber.reverseSendMessage(gsm.pack());
    } else if (id == MessageId::ADD_DEBUG_BUTTON) {
      AddDebugButtonMessage adbm(packet);
      AddDebugButtonMessage adbm2(adbm.getSubscriberId(), adbm.getButtonText(), adbm.getCategoryText(), mServerDebugSubscriber.getId());
      mServerDebugSubscriber.reverseSendMessage(adbm2.pack());
    } else if (id == MessageId::REMOVE_DEBUG_BUTTON) {
      RemoveDebugButtonMessage rdbm(packet);
      mServerDebugSubscriber.reverseSendMessage(rdbm.pack());
    } else if (id == MessageId::SOUND_LIST) {
      SoundMessage sm(packet);
      mSoundListInterface.pushMessage(sm.pack());
    } else if (id == MessageId::SERVER_READY) {
      ServerReadyMessage srm;
      mServerReadyInterface.pushMessage(srm.pack());
    } else if (id == MessageId::HEARTBEAT) {
      packet << MessageId::HEARTBEAT;
      mHeartbeatClock.restart();
      mSocket->send(packet);
    } else if (id == MessageId::MAP_DATA) {
      MapMessage mm(packet);
      mLobbyInterface.pushMessage(mm.pack());
    } else {
      TRACE_ERROR("Packet not known: " << id);
    }
  }

  if (mHeartbeatClock.getElapsedTime() > sf::milliseconds(30000)) {
    TRACE_ERROR("Received no heartbeat for 30 seconds. Lost connection to server!");
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    mGameStateSubscriber.reverseSendMessage(gsm.pack());
  }
}

void NetworkHandler::teardownSubscribersAndInterfaces() {
  teardownDebugMessages();

  mMessageHandler->unpublishInterface(Interfaces::CLIENT_SPRITE_LIST);
  mMessageHandler->unpublishInterface(Interfaces::CLIENT_LOBBY);
  mMessageHandler->unpublishInterface(Interfaces::CLIENT_SERVER_READY);
  mMessageHandler->unpublishInterface(Interfaces::CLIENT_PLAYER_DATA);
  mMessageHandler->unpublishInterface(Interfaces::CLIENT_SOUND_LIST);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_DEBUG_MENU, &mServerDebugSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::INFO_MESSAGE, &mInfoMessageSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_INPUT_LIST, &mForwaringSubscriber);
}

void NetworkHandler::handleSubscribeIpListTimeoutMessage(sf::Packet& message) {
  TRACE_ERROR("Subscribe to IpList timed out");
  mSubscribedToIpMessage = false;
}

void NetworkHandler::handleIpListMessage(sf::Packet& message) {
  IpMessage ipm(message);
  mServerIp = ipm.getIp();
  mServerPort = ipm.getPort();
  mCurrentState = STATE::Connecting;
  TRACE_INFO("Connecting socket to " << mServerIp << ", " << mServerPort);
}

void NetworkHandler::handleSubscribeGameStateTimeoutMessage(sf::Packet & message) {
  TRACE_ERROR("Subscribe to game state timed out");
}

void NetworkHandler::handleChangeGameStateMessage(sf::Packet & message) {
  GameStateMessage gsm(message);
  if (gsm.getGameState() == GAME_STATE::MAIN_MENU) {
    mCurrentState = STATE::Disconnecting;
  } else if (!mSubscribedToIpMessage && (gsm.getGameState() == GAME_STATE::LOBBY || gsm.getGameState() == GAME_STATE::JOIN)) {
    mMessageHandler->subscribeToWithTimeout(Interfaces::CLIENT_IP_LIST, &mIpSubscriber);
    mSubscribedToIpMessage = true;
  }
}