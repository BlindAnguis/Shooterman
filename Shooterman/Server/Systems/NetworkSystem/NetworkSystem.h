#pragma once

#include <thread>
#include <mutex>

#include <SFML/Network.hpp>

#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/Messages/SpriteMessage.h"
#include "../../../Common/Messages/SoundMessage.h"
#include "../../../Common/Constants.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Trace.h"

class NetworkSystem : Trace {
public:
  NetworkSystem();
  ~NetworkSystem();

  static NetworkSystem& get();
  void start();
  void shutDown();

  void addNewClientSocket(sf::TcpSocket* socket, int ID);
  void removeClientSocket(int ID);

  void setRenderData(std::shared_ptr<SpriteMessage> spriteMessage);

private:
  bool mRunning;
  bool mNumberOfPlayerChanged;
  std::mutex* mMapLock;
  std::mutex* mRenderLock;
  std::unique_ptr<std::thread> mNetworkSystemThread;
  std::map<int, std::pair<sf::TcpSocket*, std::shared_ptr<sf::Clock>>> mClientsSockets;
  std::map<int, std::pair<sf::TcpSocket*, std::shared_ptr<sf::Clock>>> mNewClientsSockets;
  std::shared_ptr<SpriteMessage> mSpriteMessage;
  Interface mPlayerDataInterface;
  Interface mGameStateInterface;
  Interface mDebugMenuInterface;
  Interface mSoundInterface;
  Interface mServerServerReadyInterface;
  Interface mPlayerLobbyInterface;
  sf::Clock mHeartbeatTimer;

  void run();
  void updateInternalMap();
  std::shared_ptr<SpriteMessage> getRenderData();

  void handleServerReadyMessage(sf::Packet& message);
  void handlePlayerDataMessage(sf::Packet& message);
  void handleAddDebugButtonMessage(sf::Packet& message);
  void handleRemoveDebugButtonMessage(sf::Packet& message);
  void handleSoundListMessage(sf::Packet& message);
  void handleChangeGameStateMessage(sf::Packet& message);
};