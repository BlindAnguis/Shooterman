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

  void setCurrentHealth(int currentHealth);
  int getCurrentHealth();

private:
  sf::TcpSocket* mSocket;
  Entity* mEntity;
  int mCurrentHealth;
};