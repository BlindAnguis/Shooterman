#include "HostListener.h"

#include "../../Common/Interfaces.h"
#include "../../Common/MessageId.h"
#include "../../Common/Messages/LobbyDataMessage.h"
#include "../../Common/Messages/PlayableCharactersMessage.h"
#include "../../Common/Messages/ChangeUsernameMessage.h"
#include "../../Common/Messages/CharacterChoosenMessage.h"
#include "../../Common/Messages/MapMessage.h"
#include "../../Common/Messages/ServerInternal/ClientDisconnectedMessage.h"
#include "../Systems/NetworkSystem/NetworkSystem.h"

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
  mSubscriber.addSignalCallback(MessageId::NEW_USERNAME, std::bind(&HostListener::handleNewUsernameMessage, this, std::placeholders::_1));
  mSubscriber.addSignalCallback(MessageId::CHARACTER_CHOOSEN, std::bind(&HostListener::handleCharacterChoosenMessage, this, std::placeholders::_1));
  mSubscriber.addSignalCallback(MessageId::CLIENT_DISCONNECTED, std::bind(&HostListener::handleClientDisconnectedMessage, this, std::placeholders::_1));
  mSubscriber.addSignalCallback(MessageId::MAP_DATA, std::bind(&HostListener::handleMapDataMessage, this, std::placeholders::_1));

  mConnectedClients = std::make_shared<std::map<int, Player*>>();
  while (!MessageHandler::get().subscribeTo(Interfaces::SERVER_PLAYER_LOBBY, &mSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  TRACE_INFO("Start to listen");
  //sf::TcpListener listener;
  sf::Socket::Status status = mListener->listen(1337, sf::IpAddress::getLocalAddress());
  mListener->setBlocking(false);
  //sf::Socket::Status status = mListener->listen(1337, "localhost");
  auto networkSystem = &NetworkSystem::get();
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
      int playerId = getNextID();
      player->setUsername("Player " + std::to_string(playerId));
      mConnectedClients->emplace(playerId, player);
      client = new sf::TcpSocket();

      // Send playable characters to new client
      PlayableCharactersMessage pcm;
      pcm.addPlayerClass(PlayerClass::Archer);
      pcm.addPlayerClass(PlayerClass::Knight);
      pcm.addPlayerClass(PlayerClass::Mage);
      pcm.addPlayerClass(PlayerClass::Spearman);
      sf::Packet pcmPacket = pcm.pack();
      player->getSocket()->send(pcmPacket);

      // Send player names to all clients, to show in lobby
      LobbyDataMessage ldm;
      for (auto client : *mConnectedClients) {
        ldm.addPlayerName(client.second->getUsername());
      }

      for (auto client : *mConnectedClients) {
        sf::Packet packet = ldm.pack();
        client.second->getSocket()->send(packet);
      }

      networkSystem->addNewClientSocket(player->getSocket(), playerId);
    }
    mSubscriber.handleMessages();

    sf::sleep(sf::milliseconds(5));
  }
  delete client; // Delete unused client socket

  MessageHandler::get().unsubscribeTo(Interfaces::SERVER_PLAYER_LOBBY, &mSubscriber);
}

void HostListener::handleNewUsernameMessage(sf::Packet& message) {
  ChangeUsernameMessage cum(message);

  (*mConnectedClients)[cum.getId()]->setUsername(cum.getUsername());

  LobbyDataMessage ldm;
  for (auto client : *mConnectedClients) {
    ldm.addPlayerName((*mConnectedClients)[client.first]->getUsername());
  }

  for (auto client : *mConnectedClients) {
    sf::Packet packet = ldm.pack();
    client.second->getSocket()->send(packet);
  }
}

void HostListener::handleCharacterChoosenMessage(sf::Packet& message) {
  CharacherChoosenMessage ccm(message);
  (*mConnectedClients)[ccm.getId()]->setPlayerClass(ccm.getPlayerClass());
}

void HostListener::handleClientDisconnectedMessage(sf::Packet& message) {
  ClientDisconnectedMessage cdm(message);
  mConnectedClients->erase(cdm.getId());

  LobbyDataMessage ldm;
  for (auto client : *mConnectedClients) {
    ldm.addPlayerName(client.second->getUsername());
  }

  for (auto client : *mConnectedClients) {
    sf::Packet packet = ldm.pack();
    client.second->getSocket()->send(packet);
  }
}

void HostListener::handleMapDataMessage(sf::Packet& message) {
  MapMessage mm(message);
  mMapData = mm.getData();
}

bool HostListener::hasMapData() {
  return mMapData.size() > 1;
}

std::array<std::array<Textures, 32>, 32> HostListener::getMapData() {
  std::array<std::array<Textures, 32>, 32> map = std::array<std::array<Textures, 32>, 32>();

  int x = 0;
  int y = 0;

  std::stringstream lineSs(mMapData);
  std::string line;

  while (std::getline(lineSs, line, '\n')) {
    std::stringstream idSs(line);
    std::string idAsString;
    while (std::getline(idSs, idAsString, ' ')) {
      map[y][x] = (Textures)std::stoi(idAsString);
      ++x;
    }
    x = 0;
    ++y;
  }

  return map;
}