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

  void setPlayerClass(PlayerClass playerClass);
  PlayerClass getPlayerClass();

private:
  std::string mUsername;
  PlayerClass mPlayerClass;
  sf::TcpSocket* mSocket;
  Entity* mEntity;
};