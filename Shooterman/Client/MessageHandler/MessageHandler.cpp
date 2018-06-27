#include "MessageHandler.h"

void MessageHandler::subscribeToSystemMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  tryToGiveId(newSubscriber);
  mSystemSubscriberList.push_back(newSubscriber);
  TRACE_INFO(STR("New subscriber (" << newSubscriber->getId() << ") added to system list"));
}

void MessageHandler::unSubscribeToSystemMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  for (auto it = mSystemSubscriberList.begin(); it != mSystemSubscriberList.end(); ++it) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mSystemSubscriberList.erase(it);
      it--;
      TRACE_INFO(STR("Removed subscriber (" << newSubscriber->getId() << ") from to system list"));
    }
  }
}

void MessageHandler::pushSystemMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  for (Subscriber* subscriber : mSystemSubscriberList) {
    subscriber->sendMessage(message);
  }
}


void MessageHandler::subscribeToInputMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mInputSubscriberLock);
  tryToGiveId(newSubscriber);
  mInputSubscriberList.push_back(newSubscriber);
  TRACE_INFO(STR("New subscriber (" << newSubscriber->getId() << ") added to input list"));
}

void MessageHandler::unSubscribeToInputMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mInputSubscriberLock);
  for (auto it = mInputSubscriberList.begin(); it != mInputSubscriberList.end(); ++it) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mInputSubscriberList.erase(it);
      it--;
      TRACE_INFO(STR("Removed subscriber (" << newSubscriber->getId() << ") from to input list"));
    }
  }
}

void MessageHandler::pushInputMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mInputSubscriberLock);
  for (Subscriber* subscriber : mInputSubscriberList) {
    subscriber->sendMessage(message);
  }
}


void MessageHandler::subscribeToSpriteListMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSpriteListSubscriberLock);
  tryToGiveId(newSubscriber);
  mSpriteListSubscriberList.push_back(newSubscriber);
  TRACE_INFO(STR("New subscriber (" << newSubscriber->getId() << ") added to sprite list"));
}

void MessageHandler::unSubscribeToSpriteListMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSpriteListSubscriberLock);
  for (auto it = mSpriteListSubscriberList.begin(); it != mSpriteListSubscriberList.end(); ++it) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mSpriteListSubscriberList.erase(it);
      it--;
      TRACE_INFO(STR("Removed subscriber (" << newSubscriber->getId() << ") from to sprite list"));
    }
  }
}

void MessageHandler::pushSpriteListMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSpriteListSubscriberLock);
  for (Subscriber* subscriber : mSpriteListSubscriberList) {
    subscriber->sendMessage(message);
  }
}


void MessageHandler::unsubscribeAll(Subscriber* subscriber) {
  unSubscribeToSystemMessages(subscriber);
  unSubscribeToInputMessages(subscriber);
  unSubscribeToSpriteListMessages(subscriber);
}

void MessageHandler::tryToGiveId(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mIdGeneratorLock);
  if (subscriber->getId() == -1) {
    subscriber->setId(mCurrentId);
    mCurrentId++;
  }
}