#pragma once

#include <thread>
#include <mutex>

#include <SFML/Network.hpp>

#include "../../../Client/MessageHandler/MessageHandler.h"
#include "../../../Common/Constants.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Trace.h"
#include "../../../Common/Socket/Tcp.h"

class NetworkSystem : Trace {
public:
  NetworkSystem();
  ~NetworkSystem();

  void start();
  void shutDown();

  void addNewClientSocket(sf::TcpSocket* socket, int ID);
  void removeClientSocket(int ID);

private:
  bool mRunning = true;
  std::mutex* mMapLock;
  std::unique_ptr<std::thread> mNetworkSystemThread;
  std::map<int, sf::TcpSocket*> mClientsSockets;
  std::map<int, sf::TcpSocket*> mNewClientsSockets;

  void startup();
  void updateInternalMap();

};