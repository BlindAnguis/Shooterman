#pragma once

#include <thread>
#include <SFML\Network.hpp>
#include "../../Common/Trace.h"

class HostListener : Trace
{
public:
  HostListener();
  ~HostListener();
  void startListening();
  void stopListening();
private:
  void listen();
  std::thread* mHostListenerThread;
  bool mRunning = false;
};

