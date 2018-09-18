#include "HostListener.h"

HostListener::HostListener() {
  mName = "[SERVER: HOST_LISTENER]";
  TRACE_INFO("Created a HostListener");
}

HostListener::~HostListener() {
}

void HostListener::startListening() {
  TRACE_INFO("Created a thread for listening");
  mHostListenerThread = new std::thread(&HostListener::listen, this);
}

void HostListener::stopListening() {
  TRACE_INFO("Start to stop listening");
  mRunning = false;
  mHostListenerThread->join();
  delete mHostListenerThread;
  TRACE_INFO("Stopped listening");
}

void HostListener::listen() {
  TRACE_INFO("Start to listen");
  sf::TcpListener listener;
  if (!listener.listen(1337))
  {
    TRACE_ERROR("Couldn't start listening to port 1337");
    return;
  }
  mRunning = true;
  while (mRunning) {
    sf::TcpSocket client;
    client.setBlocking(false);
    TRACE_INFO("Searching for client");
    if (listener.accept(client) == sf::Socket::Done)
    {
      // A new client just connected!
      std::cout << "New connection received from " << client.getRemoteAddress() << std::endl;
      //doSomethingWith(client);
    }
  }
  TRACE_INFO("Stopped listening");
}
