#pragma once

#include <thread>
#include <list>
#include <array>
#include <SFML\Network.hpp>
#include "../../Common/Trace.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../EntityManager/EntityManager.h"
#include "../Player.h"

class HostListener : Trace
{
public:
  HostListener(std::shared_ptr<MessageHandler> messageHandler);
  ~HostListener();
  void startListening();
  std::shared_ptr<std::map<int, Player*>> stopListening();
  bool isListening();

  bool hasMapData();
  std::array<std::array<Textures, 32>, 32> getMapData();

private:
  void listen();
  int getNextID() {
    return ++lastID;
  }
  int lastID = 0;
  std::thread* mHostListenerThread;
  bool mRunning = false;
  sf::TcpListener* mListener;
  Subscriber mSubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::shared_ptr<std::map<int, Player*>> mConnectedClients;
  std::string mMapName;
  std::string mMapData;

  void handleNewUsernameMessage(sf::Packet& message);
  void handleCharacterChoosenMessage(sf::Packet& message);
  void handleClientDisconnectedMessage(sf::Packet& message);
  void handleMapDataMessage(sf::Packet& message);
};

