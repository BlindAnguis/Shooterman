#include "MessageHandler.h"

// SYSTEM
void MessageHandler::subscribeToSystemMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  tryToGiveId(newSubscriber);
  mSystemSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to system list");
}

void MessageHandler::unSubscribeToSystemMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  for (auto it = mSystemSubscriberList.begin(); it != mSystemSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mSystemSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to system list");
    } else {
      ++it;
    }
  }
}

void MessageHandler::pushSystemMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  for (Subscriber* subscriber : mSystemSubscriberList) {
    subscriber->sendMessage(message);
  }
}

// Sound LIST
void MessageHandler::subscribeToSoundMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSoundSubscriberLock);
  tryToGiveId(newSubscriber);
  mSoundSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to sound list");
}

void MessageHandler::unSubscribeToSoundMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSoundSubscriberLock);
  for (auto it = mSoundSubscriberList.begin(); it != mSoundSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mSoundSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to sound list");
    } else {
      ++it;
    }
  }
}

void MessageHandler::pushSoundMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSoundSubscriberLock);
  for (Subscriber* subscriber : mSoundSubscriberList) {
    subscriber->sendMessage(message);
  }
}

void MessageHandler::unsubscribeAll(Subscriber* subscriber) {
  unSubscribeToSystemMessages(subscriber);
  unSubscribeToSoundMessages(subscriber);
}

void MessageHandler::tryToGiveId(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mIdGeneratorLock);
  if (subscriber->getId() == -1) {
    subscriber->setId(mCurrentId);
    mCurrentId++;
  }
}

void MessageHandler::publishInterface(std::string name, Interface* pc) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto it = mPublishedComms.find(name);
  if (it == mPublishedComms.end()) {
    pc->setMName(name);
    mPublishedComms.emplace(name, pc);
    TRACE_DEBUG("Interface: " << name << " published");
  }
  else {
    TRACE_WARNING("Interface: " << name << " is already published");
  }
}

void MessageHandler::unpublishInterface(std::string name) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto interface = mPublishedComms.find(name);
  if (interface != mPublishedComms.end()) {
    while (interface->second->getSubscribers().size() > 0) {
      interface->second->unsubscribe(interface->second->getSubscribers().front());
    }
    mPublishedComms.erase(interface);
  }
}

bool MessageHandler::subscribeTo(std::string name, Subscriber* s) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto it = mPublishedComms.find(name);
  if (it != mPublishedComms.end()) {
    it->second->subscribe(s);
    return true;
  }
  return false;
}

void MessageHandler::unsubscribeTo(std::string name, Subscriber* s) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto it = mPublishedComms.find(name);
  if (it != mPublishedComms.end()) {
    it->second->unsubscribe(s);
  } else {
    TRACE_INFO("Could not find " << name);
  }
}