#include "MessageHandler.h"

#include <iostream>

void MessageHandler::subscribeToSystemMessages(Subscriber* newSubscriber) {
  std::cout << "[MESSAGEHANDLER] New subscriber added to system list" << std::endl;
  m_systemSubscriberList.push_back(newSubscriber);
}

void MessageHandler::pushSystemMessage(sf::Packet message) {
  for (Subscriber* subscriber : m_systemSubscriberList) {
    subscriber->sendMessage(message);
  }
}

void MessageHandler::subscribeToInputMessages(Subscriber* newSubscriber) {
  std::cout << "[MESSAGEHANDLER] New subscriber added to input list" << std::endl;
  m_inputSubscriberList.push_back(newSubscriber);
}

void MessageHandler::pushInputMessage(sf::Packet message) {
  for (Subscriber* subscriber : m_inputSubscriberList) {
    subscriber->sendMessage(message);
  }
}

void MessageHandler::subscribeToSpriteListMessages(Subscriber* newSubscriber) {
  std::cout << "[MESSAGEHANDLER] New subscriber added to sprite list list" << std::endl;
  m_spriteListSubscriberList.push_back(newSubscriber);
}

void MessageHandler::pushSpriteListMessage(sf::Packet message) {
  for (Subscriber* subscriber : m_spriteListSubscriberList) {
    subscriber->sendMessage(message);
  }
}

sf::Packet MessageHandler::readInputMessage() {
  sf::Packet message;
  message << -1;

  //std::cout << "Queue size: " << m_inputQueue.size() << std::endl;

  m_inputQueueMutex.lock();
  if (!m_inputQueue.empty()) {
    message = m_inputQueue.front();
    m_inputQueue.pop();
  }
  m_inputQueueMutex.unlock();
  return message;
}

sf::Packet MessageHandler::readGraphicsMessage() {
  sf::Packet message;
  message << -1;
  m_graphicsQueueMutex.lock();
  if (!m_graphicsQueue.empty()) {
    message = m_graphicsQueue.front();
    m_graphicsQueue.pop();
  }
  m_graphicsQueueMutex.unlock();
  return message;
}