#include "HostListener.h"

#include "../../Common/MessageId.h"
#include "../../Common/Messages/LobbyDataMessage.h"
#include "../../Common/Messages/PlayableCharactersMessage.h"
#include "../../Common/Messages/ChangeUsernameMessage.h"
#include "../../Common/Messages/CharacterChoosenMessage.h"
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
  mConnectedClients = std::make_shared<std::map<int, Player*>>();
  while (!MessageHandler::get().subscribeTo("ServerPlayerLobby", &mPlayerLobbyDataSubscriber)) {
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
    sf::sleep(sf::milliseconds(5));
    handlePlayerLobbyData();
  }
  delete client; // Delete unused client socket

  MessageHandler::get().unsubscribeTo("ServerPlayerLobby", &mPlayerLobbyDataSubscriber);
}

void HostListener::handlePlayerLobbyData() {
  auto playerLobbyDataMessages = mPlayerLobbyDataSubscriber.getMessageQueue();
  while (playerLobbyDataMessages.size() > 0) {
    sf::Packet lobbyPacket = playerLobbyDataMessages.front();
    playerLobbyDataMessages.pop();

    int id = -1;
    lobbyPacket >> id;

    switch (id) {
    case NEW_USERNAME: {
      ChangeUsernameMessage cum(lobbyPacket);
      
      (*mConnectedClients)[cum.getId()]->setUsername(cum.getUsername());

      LobbyDataMessage ldm;
      for (auto client : *mConnectedClients) {
        ldm.addPlayerName((*mConnectedClients)[client.first]->getUsername());
      }

      for (auto client : *mConnectedClients) {
        sf::Packet packet = ldm.pack();
        client.second->getSocket()->send(packet);
      }
      break;
    }
    break;
    case CHARACTER_CHOOSEN: {
      CharacherChoosenMessage ccm(lobbyPacket);
      (*mConnectedClients)[ccm.getId()]->setPlayerClass(ccm.getPlayerClass());
    }
    break;
    default:
      TRACE_ERROR("Received unknown message: " << id);
      break;
    }
  }
}