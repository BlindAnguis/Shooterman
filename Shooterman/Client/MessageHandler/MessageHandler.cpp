#include "MessageHandler.h"

#include <iostream>

void MessageHandler::subscribeToSystemMessages(Subscriber* newSubscriber) {
  std::cout << "[MESSAGEHANDLER] New subscriber added to system list" << std::endl;
  mSystemSubscriberList.push_back(newSubscriber);
}

void MessageHandler::pushSystemMessage(sf::Packet message) {
  for (Subscriber* subscriber : mSystemSubscriberList) {
    subscriber->sendMessage(message);
  }
}

void MessageHandler::subscribeToInputMessages(Subscriber* newSubscriber) {
  std::cout << "[MESSAGEHANDLER] New subscriber added to input list" << std::endl;
  mInputSubscriberList.push_back(newSubscriber);
}

void MessageHandler::pushInputMessage(sf::Packet message) {
  for (Subscriber* subscriber : mInputSubscriberList) {
    subscriber->sendMessage(message);
  }
}

void MessageHandler::subscribeToSpriteListMessages(Subscriber* newSubscriber) {
  std::cout << "[MESSAGEHANDLER] New subscriber added to sprite list list" << std::endl;
  mSpriteListSubscriberList.push_back(newSubscriber);
}

void MessageHandler::pushSpriteListMessage(sf::Packet message) {
  for (Subscriber* subscriber : mSpriteListSubscriberList) {
    subscriber->sendMessage(message);
  }
}