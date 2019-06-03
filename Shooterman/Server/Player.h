#pragma once

#include <SFML/Network.hpp>

#include "EntityManager/EntityManager.h"

class Player {
public:
  Player();
  ~Player();

  void setSocket(sf::TcpSocket* newSocket);
  sf::TcpSocket* getSocket();
  
  void setEntity(Entity* newEntity);
  Entity* getEntity();

  void setUsername(std::string username);
  std::string getUsername();

private:
  std::string mUsername;
  sf::TcpSocket* mSocket;
  Entity* mEntity;
};