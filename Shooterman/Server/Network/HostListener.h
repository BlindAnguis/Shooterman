#pragma once

#include <thread>
#include <list>
#include <SFML\Network.hpp>
#include "../../Common/Trace.h"
#include "../EntityManager/EntityManager.h"

class HostListener : Trace
{
public:
  HostListener();
  ~HostListener();
  void startListening();
  std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> stopListening();
  bool isListening();
private:
  void listen();
  int getNextID() {
    return ++lastID;
  }
  int lastID = 0;
  std::thread* mHostListenerThread;
  bool mRunning = false;
  sf::TcpListener* mListener;
  std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> mConnectedClients;
};

