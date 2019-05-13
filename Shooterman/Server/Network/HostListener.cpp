#include "HostListener.h"

#include "../../Common/Messages/LobbyDataMessage.h"

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

std::shared_ptr<std::map<int, Player*>> HostListener::stopListening() {
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
  mConnectedClients = std::make_shared<std::map<int, Player*>>();
  TRACE_INFO("Start to listen");
  //sf::TcpListener listener;
  sf::Socket::Status status = mListener->listen(1337, sf::IpAddress::getLocalAddress());
  mListener->setBlocking(false);
  //sf::Socket::Status status = mListener->listen(1337, "localhost");
  if (status != sf::Socket::Status::Done) {
    TRACE_ERROR("Couldn't start listening to port 1337 on IP: " << sf::IpAddress::getLocalAddress() << ", status: " << status);
    //TRACE_ERROR("Couldn't start listening to port 1337 on IP: localhost, status: " << status);
    return;
  }

  TRACE_INFO("Started listening to port: 1337 and IP: " << sf::IpAddress::getLocalAddress());
  //TRACE_INFO("Started listening to port: 1337 and IP: localhost");
  mRunning = true;
  TRACE_INFO("Searching for clients");
  sf::TcpSocket* client = new sf::TcpSocket();
  while (mRunning) {
    if (mListener->accept(*client) == sf::Socket::Status::Done) {
      // A new client just connected!
      TRACE_INFO("New connection received from " << client->getRemoteAddress());
      Player* player = new Player();
      player->setSocket(client);
      mConnectedClients->emplace(getNextID(), player);
      //doSomethingWith(client);
      // TODO: Add client to a list of clients?
      client = new sf::TcpSocket();


      // Send player names to all clients, to show in lobby
      LobbyDataMessage ldm;
      for (auto client : *mConnectedClients) {
        ldm.addPlayerName("Dummy name " + std::to_string(client.first - 1));
      }

      for (auto client : *mConnectedClients) {
        sf::Packet packet = ldm.pack();
        client.second->getSocket()->send(packet);
      }
    }
    sf::sleep(sf::milliseconds(5));
  }
  delete client; // Delete unused client socket
}
