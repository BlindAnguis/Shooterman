#include "HostListener.h"

HostListener::HostListener() {
  mName = "SERVER: HOST_LISTENER";
  TRACE_INFO("Created a HostListener");
  mListener = new sf::TcpListener;
}

HostListener::~HostListener() {
  delete mListener;
}

void HostListener::startListening() {
  TRACE_INFO("Created a thread for listening");
  mHostListenerThread = new std::thread(&HostListener::listen, this);
}

std::list<sf::TcpSocket*> HostListener::stopListening() {
  TRACE_INFO("Start to stop listening");
  mRunning = false;
  mListener->close();
  mHostListenerThread->join();
  TRACE_INFO("Stopped listening");
  delete mHostListenerThread;
  return mConnectedClients;
}

bool HostListener::isListening() {
  return mRunning;
}

void HostListener::listen() {
  TRACE_INFO("Start to listen");
  //sf::TcpListener listener;
  sf::Socket::Status status = mListener->listen(1337, sf::IpAddress::getLocalAddress());
  //sf::Socket::Status status = mListener->listen(1337, "localhost");
  if (status != sf::Socket::Status::Done) {
    TRACE_ERROR("Couldn't start listening to port 1337 on IP: " << sf::IpAddress::getLocalAddress() << ", status: " << status);
    //TRACE_ERROR("Couldn't start listening to port 1337 on IP: localhost, status: " << status);
    return;
  }

  TRACE_INFO("Started listening to port: 1337 and IP: " << sf::IpAddress::getLocalAddress());
  //TRACE_INFO("Started listening to port: 1337 and IP: localhost");
  mRunning = true;
  while (mRunning) {
    sf::TcpSocket* client = new sf::TcpSocket();
    //listener.setBlocking(false);
    TRACE_INFO("Searching for client");
    if (mListener->accept(*client) == sf::Socket::Status::Done) {
      // A new client just connected!
      std::cout << "New connection received from " << client->getRemoteAddress() << std::endl;
      mConnectedClients.push_back(client);
      //doSomethingWith(client);
      // TODO: Add client to a list of clients?
    }
  }
}
