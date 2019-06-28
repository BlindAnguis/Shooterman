#include "NetworkHandler.h"

#include "../../Common/KeyBindings.h"
#include "../../Common/Constants.h"
#include "../../Common/MessageId.h"
#include "../../Common/Interfaces.h"
#include "../../Common/MessageHandler/MessageHandler.h"
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

NetworkHandler::NetworkHandler() {
  mName = "CLIENT: NETWORK_HANDLER";
}

NetworkHandler::~NetworkHandler() {
  TRACE_FUNC_ENTER();
  TRACE_FUNC_EXIT();
}

void NetworkHandler::start() {
  TRACE_INFO("Starting module...");
  mNetworkHandlerThread = std::make_unique<std::thread>(&NetworkHandler::run, this);
  TRACE_INFO("Starting module done");
}

void NetworkHandler::run() {
  TRACE_FUNC_ENTER();
  setupSubscribersAndInterfaces();

  CONNECTION_STATUS connectionStatus = setupSocketConnection();

  if (connectionStatus == CONNECTION_STATUS::Cancel) {
    MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateSubscriber);
    return;
  }

  // Failed to connect to server
  if (connectionStatus != CONNECTION_STATUS::Done) {
    TRACE_ERROR("Connection failed! " << connectionStatus);
    InfoMessage msg("Connection failed.", 3);
    mInfoMessageSubscriber.reverseSendMessage(msg.pack());
    
    GameStateMessage gsm(GAME_STATE::JOIN);
    mGameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateSubscriber);
    teardownDebugMessages();
    return;
  }

  TRACE_INFO("Connected!");

  MessageHandler::get().publishInterface(Interfaces::CLIENT_SPRITE_LIST, &mSpriteListInterface);
  MessageHandler::get().publishInterface(Interfaces::CLIENT_PLAYER_DATA, &mPlayerDataInterface);
  MessageHandler::get().publishInterface(Interfaces::CLIENT_SOUND_LIST, &mSoundListInterface);
  MessageHandler::get().subscribeTo(Interfaces::CLIENT_INPUT_LIST, &mMessageSubscriber);
  MessageHandler::get().subscribeTo(Interfaces::CLIENT_DEBUG_MENU, &mServerDebugSubscriber);

  mSocket.setBlocking(false);
  mRunning = true;

  handlePackets();
 
  mSocket.disconnect();
  teardownSubscribersAndInterfaces();
  TRACE_FUNC_EXIT();
}

void NetworkHandler::setupSubscribersAndInterfaces() {
  TRACE_FUNC_ENTER();
  MessageHandler::get().publishInterface(Interfaces::CLIENT_LOBBY, &mLobbyInterface);
  MessageHandler::get().publishInterface(Interfaces::CLIENT_SERVER_READY, &mServerReadyInterface);

  while (!setupDebugMessages("Client", "Network")) {
    sf::sleep(sf::milliseconds(5));
  }

  while (!MessageHandler::get().subscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }

  while (!MessageHandler::get().subscribeTo(Interfaces::CLIENT_IP_LIST, &mMessageSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  while (!MessageHandler::get().subscribeTo(Interfaces::INFO_MESSAGE, &mInfoMessageSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  TRACE_FUNC_EXIT();
}

CONNECTION_STATUS NetworkHandler::setupSocketConnection() {
  TRACE_FUNC_ENTER();
  CONNECTION_STATUS connectionStatus = CONNECTION_STATUS::Cancel;

  // Wait for IP address
  std::queue<sf::Packet> messages;
  while (messages.size() == 0) {
    messages = mMessageSubscriber.getMessageQueue();
    sf::sleep(sf::milliseconds(5));
  }
  // We have the IP, no need to be subscribed
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_IP_LIST, &mMessageSubscriber);

  int ID = -1;
  auto ipMessage = messages.front();
  ipMessage >> ID;
  if (ID != MessageId::IP_MESSAGE) {
    if (ID != MessageId::SHUT_DOWN) {
      // Received unexpected message
      TRACE_ERROR("Received unexpected message with ID: " << ID);
      GameStateMessage gsm(GAME_STATE::PREVIOUS);
      mGameStateSubscriber.reverseSendMessage(gsm.pack());
    }
    MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateSubscriber);
    return connectionStatus;
  }

  IpMessage ipm(ipMessage);
  TRACE_INFO("Connecting socket to " << ipm.getIp());
  int connectionAttempts = 10;
  // Try to connect multiple times
  while (connectionStatus != sf::Socket::Status::Done && connectionAttempts > 0) {
    connectionStatus = (CONNECTION_STATUS)mSocket.connect(sf::IpAddress(ipm.getIp()), ipm.getPort(), sf::milliseconds(100));
    connectionAttempts--;
    sf::sleep(sf::milliseconds(500));

    if (!checkIfConnectionIsCancelled()) {
      return CONNECTION_STATUS::Cancel;
    }
  }

  TRACE_FUNC_EXIT();
  return connectionStatus;
}

bool NetworkHandler::checkIfConnectionIsCancelled() {
  auto gameStateMessageQueue = mGameStateSubscriber.getMessageQueue();
  sf::Packet packet;
  while (!gameStateMessageQueue.empty()) {
    packet = gameStateMessageQueue.front();
    gameStateMessageQueue.pop();
  }
  if (packet.getDataSize() > 0) {
    int id;
    packet >> id;
    if (id == MessageId::CHANGE_GAME_STATE) {
      GameStateMessage gsm(packet);
      if (gsm.getGameState() != GAME_STATE::CLIENT_LOBBY) {
        return false;
      }
    }
  }
  return true;
}

void NetworkHandler::handlePackets() {
  TRACE_FUNC_ENTER();

  mHeartbeatClock.restart();

  while (mRunning) {
    auto systemMessageQueue = mMessageSubscriber.getMessageQueue();
    while (!systemMessageQueue.empty()) {
      sf::Packet packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      mSocket.send(packet);
    }

    auto gameStateMessageQueue = mGameStateSubscriber.getMessageQueue();
    while (!gameStateMessageQueue.empty()) {
      sf::Packet packet = gameStateMessageQueue.front();
      gameStateMessageQueue.pop();
      mSocket.send(packet);
    }

    auto lobbyMessageQueue = mLobbyInterface.getMessageQueue();
    while (!lobbyMessageQueue.empty()) {
      sf::Packet packet = lobbyMessageQueue.front();
      lobbyMessageQueue.pop();
      mSocket.send(packet);
    }

    auto debugMessageQueue = mServerDebugSubscriber.getMessageQueue();
    while (!debugMessageQueue.empty()) {
      sf::Packet packet = debugMessageQueue.front();
      debugMessageQueue.pop();
      int id = -1;
      packet >> id;

      AddDebugButtonMessage adbm(packet);
      sf::Packet p2 = adbm.pack();
      mSocket.send(p2);
    }

    sf::Packet packet;
    if (mSocket.receive(packet) == sf::Socket::Done) {
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
        mSocket.send(packet);
      } else {
        TRACE_ERROR("Packet not known: " << id);
      }
    }
    sf::sleep(sf::milliseconds(1));

    if (mHeartbeatClock.getElapsedTime() > sf::milliseconds(30000)) {
      TRACE_ERROR("Received no heartbeat for 30 seconds. Lost connection to server!");
      mRunning = false;
      GameStateMessage gsm(GAME_STATE::MAIN_MENU);
      mGameStateSubscriber.reverseSendMessage(gsm.pack());
    }
  }
  TRACE_FUNC_EXIT();
}

void NetworkHandler::teardownSubscribersAndInterfaces() {
  TRACE_FUNC_ENTER();
  teardownDebugMessages();

  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_SPRITE_LIST);
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_LOBBY);
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_SERVER_READY);
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_PLAYER_DATA);
  MessageHandler::get().unpublishInterface(Interfaces::CLIENT_SOUND_LIST);
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_INPUT_LIST, &mMessageSubscriber);
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_DEBUG_MENU, &mServerDebugSubscriber);
  MessageHandler::get().unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mGameStateSubscriber);
  MessageHandler::get().unsubscribeTo(Interfaces::INFO_MESSAGE, &mInfoMessageSubscriber);
  TRACE_FUNC_EXIT()
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  TRACE_INFO("Shutdown of module done");
}
