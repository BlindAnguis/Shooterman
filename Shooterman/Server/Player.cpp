#include "Player.h"

Player::Player() {
}


Player::~Player() {
}

void Player::setSocket(sf::TcpSocket* newSocket) {
  mSocket = newSocket;
}

sf::TcpSocket* Player::getSocket() {
  return mSocket;
}

void Player::setEntity(Entity* newEntity) {
  mEntity = newEntity;
}

Entity* Player::getEntity() {
  return mEntity;
}

void Player::setUsername(std::string username) {
  mUsername = username;
}

std::string Player::getUsername() {
  return mUsername;
}

void Player::setPlayerClass(PlayerClass playerClass) {
  mPlayerClass = playerClass;
}

PlayerClass Player::getPlayerClass() {
  return mPlayerClass;
}