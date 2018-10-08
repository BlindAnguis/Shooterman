#pragma once

#include <thread>
#include <list>
#include <SFML\Network.hpp>
#include "../../Common/Trace.h"

class HostListener : Trace
{
public:
  HostListener();
  ~HostListener();
  void startListening();
  std::list<sf::TcpSocket*> stopListening();
  bool isListening();
private:
  void listen();
  std::thread* mHostListenerThread;
  bool mRunning = false;
  sf::TcpListener* mListener;
  std::list<sf::TcpSocket*> mConnectedClients;
};

