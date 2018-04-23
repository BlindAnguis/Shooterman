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