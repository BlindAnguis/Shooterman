#include "MessageHandler.h"

#include <iostream>

MessageHandler::MessageHandler() {
  std::cout << "Message handler started" << std::endl;
}

void MessageHandler::publishInputMessage(sf::Packet message) {
  m_inputQueueMutex.lock();
  m_inputQueue.push(message);
  m_inputQueueMutex.unlock();
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

void MessageHandler::publishGraphicsMessage(sf::Packet message) {
  m_graphicsQueueMutex.lock();
  m_graphicsQueue.push(message);
  m_graphicsQueueMutex.unlock();
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