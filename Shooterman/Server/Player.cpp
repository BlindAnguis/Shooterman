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

void Player::setCurrentHealth(int currentHealth) {
  mCurrentHealth = currentHealth;
}

int Player::getCurrentHealth() {
  return mCurrentHealth;
}