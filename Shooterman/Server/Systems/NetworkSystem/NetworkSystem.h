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
#include "../../../Common/Socket/Tcp.h"

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
  bool mRunning = true;
  std::mutex* mMapLock;
  std::mutex* mRenderLock;
  std::unique_ptr<std::thread> mNetworkSystemThread;
  std::map<int, sf::TcpSocket*> mClientsSockets;
  std::map<int, sf::TcpSocket*> mNewClientsSockets;
  std::shared_ptr<SpriteMessage> mSpriteMessage;
  Interface mPlayerDataInterface;
  Interface mGameStateInterface;
  Interface mDebugMenuInterface;
  Interface mSoundInterface;
  Interface mServerServerReadyInterface;
  Interface mPlayerLobbyInterface;

  void startup();
  void updateInternalMap();
  std::shared_ptr<SpriteMessage> getRenderData();
  void handlePlayerData();
  void handleDebugMenu();
};