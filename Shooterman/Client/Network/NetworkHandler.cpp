#include "NetworkHandler.h"

#include "../../Common/KeyBindings.h"
#include "../../Common/Constants.h"
#include "../../Common/MessageId.h"
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

  sf::Socket::Status connectionStatus = setupSocketConnection();

  // Failed to connect to server
  if (connectionStatus != sf::Socket::Status::Done) {
    TRACE_ERROR("Connection failed! " << connectionStatus);
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    mGameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("ClientGameState", &mGameStateSubscriber);
    teardownDebugMessages();
    return;
  }

  TRACE_INFO("Connected!");

  MessageHandler::get().publishInterface("ClientSpriteList", &mSpriteListInterface);
  MessageHandler::get().publishInterface("ClientPlayerData", &mPlayerDataInterface);
  MessageHandler::get().publishInterface("ClientSoundList", &mSoundListInterface);
  MessageHandler::get().subscribeTo("ClientInputList", &mMessageSubscriber);
  MessageHandler::get().subscribeTo("ClientDebugMenu", &mServerDebugSubscriber);

  mSocket.setBlocking(false);
  mRunning = true;

  handlePackets();
 
  mSocket.disconnect();
  teardownSubscribersAndInterfaces();
  TRACE_FUNC_EXIT();
}

void NetworkHandler::setupSubscribersAndInterfaces() {
  TRACE_FUNC_ENTER();
  MessageHandler::get().publishInterface("ClientLobby", &mLobbyInterface);
  MessageHandler::get().publishInterface("ClientServerReady", &mServerReadyInterface);

  while (!setupDebugMessages("Client", "Network")) {
    sf::sleep(sf::milliseconds(5));
  }

  while (!MessageHandler::get().subscribeTo("ClientGameState", &mGameStateSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }

  while (!MessageHandler::get().subscribeTo("ClientIpList", &mMessageSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  TRACE_FUNC_EXIT();
}

sf::Socket::Status NetworkHandler::setupSocketConnection() {
  TRACE_FUNC_ENTER();
  sf::Socket::Status connectionStatus = sf::Socket::Status::Disconnected;

  // Wait for IP address
  std::queue<sf::Packet> messages;
  while (messages.size() == 0) {
    messages = mMessageSubscriber.getMessageQueue();
    sf::sleep(sf::milliseconds(5));
  }
  // We have the IP, no need to be subscribed
  MessageHandler::get().unsubscribeTo("ClientIpList", &mMessageSubscriber);

  int ID = -1;
  auto ipMessage = messages.front();
  ipMessage >> ID;
  if (ID != IP_MESSAGE) {
    if (ID != SHUT_DOWN) {
      // Received unexpected message
      TRACE_ERROR("Received unexpected message with ID: " << ID);
      GameStateMessage gsm(GAME_STATE::PREVIOUS);
      mGameStateSubscriber.reverseSendMessage(gsm.pack());
    }
    MessageHandler::get().unsubscribeTo("ClientGameState", &mGameStateSubscriber);
    return connectionStatus;
  }

  IpMessage ipm(ipMessage);
  TRACE_INFO("Connecting socket to " << ipm.getIp());
  int connectionAttempts = 10;
  // Try to connect multiple times
  while (connectionStatus != sf::Socket::Status::Done && connectionAttempts > 0) {
    connectionStatus = mSocket.connect(sf::IpAddress(ipm.getIp()), ipm.getPort(), sf::milliseconds(100));
    connectionAttempts--;
    sf::sleep(sf::milliseconds(500));
  }

  TRACE_FUNC_EXIT();
  return connectionStatus;
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
      if (id == SPRITE_LIST_CACHE) {
        SpriteCacheMessage sm(packet);
        mSpriteListInterface.pushMessage(sm.pack());
      } else if (id == SPRITE_LIST) {
        SpriteMessage sm(packet);
        mSpriteListInterface.pushMessage(sm.pack());
      } else if (id == PLAYER_DATA) {
        PlayerDataMessage pdm(packet);
        mPlayerDataInterface.pushMessage(pdm.pack());
      } else if (id == PLAYER_USERNAMES) {
        LobbyDataMessage ldm(packet);
        mLobbyInterface.pushMessage(ldm.pack());
      } else if (id == PLAYABLE_CHARACTERS) {
        PlayableCharactersMessage pcm(packet);
        mLobbyInterface.pushMessage(pcm.pack());
      } else if (id == CHANGE_GAME_STATE) {
        GameStateMessage gsm(packet);
        mGameStateSubscriber.reverseSendMessage(gsm.pack());
      } else if (id == ADD_DEBUG_BUTTON) {
        AddDebugButtonMessage adbm(packet);
        AddDebugButtonMessage adbm2(adbm.getSubscriberId(), adbm.getButtonText(), adbm.getCategoryText(), mServerDebugSubscriber.getId());
        mServerDebugSubscriber.reverseSendMessage(adbm2.pack());
      } else if (id == REMOVE_DEBUG_BUTTON) {
        RemoveDebugButtonMessage rdbm(packet);
        mServerDebugSubscriber.reverseSendMessage(rdbm.pack());
      } else if (id == SOUND_LIST) {
        SoundMessage sm(packet);
        mSoundListInterface.pushMessage(sm.pack());
      } else if (id == SERVER_READY) {
        ServerReadyMessage srm;
        mServerReadyInterface.pushMessage(srm.pack());
      } else if (id == HEARTBEAT) {
        packet << HEARTBEAT;
        //mHeartbeatClock.restart();
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

  MessageHandler::get().unpublishInterface("ClientSpriteList");
  MessageHandler::get().unpublishInterface("ClientLobby");
  MessageHandler::get().unpublishInterface("ClientServerReady");
  MessageHandler::get().unpublishInterface("ClientPlayerData");
  MessageHandler::get().unpublishInterface("ClientSoundList");
  MessageHandler::get().unsubscribeTo("ClientInputList", &mMessageSubscriber);
  MessageHandler::get().unsubscribeTo("ClientDebugMenu", &mServerDebugSubscriber);
  MessageHandler::get().unsubscribeTo("ClientGameState", &mGameStateSubscriber);
  TRACE_FUNC_EXIT()
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  TRACE_INFO("Shutdown of module done");
}
